package edu.sustech.cs307.storage;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.storage.replacer.LRUReplacer;
import edu.sustech.cs307.storage.replacer.ClockReplacer;

import edu.sustech.cs307.storage.replacer.PageReplacer;

import java.util.*;

/**
 * BufferPool 类实现了一个缓冲池，用于管理页面的缓存。
 * 它支持从磁盘读取页面、将页面写回磁盘、以及管理页面的固定和释放。
 * 
 * 主要功能包括：
 * - 从缓冲池获取页面（FetchPage）
 * - 取消页面的固定（unpin_page）
 * - 将页面写回磁盘（FlushPage）
 * - 创建新页面（NewPage）
 * - 删除页面（DeletePage）
 * - 刷新所有页面（FlushAllPages）
 * - 删除指定文件的所有页面（DeleteAllPages）
 */
public class BufferPool {
    private final int poolSize;
    // frames
    private final ArrayList<Page> pages;

    // PagePosition -> frame_id
    private final HashMap<PagePosition, Integer> pageMap;
    private final LinkedList<Integer> freeList;
    private final DiskManager diskManager;
    private final PageReplacer replacer;

    /**
     * 构造一个 BufferPool 实例。
     *
     * @param pool_size   缓冲池的大小
     * @param diskManager 磁盘管理器，用于管理磁盘操作
     */
    public BufferPool(int pool_size, DiskManager diskManager) {
        this(pool_size, diskManager,new LRUReplacer(pool_size));
    }

    public BufferPool(int pool_size, DiskManager diskManager, PageReplacer replacer) {
        this.poolSize = pool_size;
        this.replacer = replacer;
        this.freeList = new LinkedList<>();
        for (int i = 0; i < pool_size; i++) {
            freeList.add(i);
        }
        this.pageMap = new HashMap<>();
        this.pages = new ArrayList<>();
        for (int i = 0; i < pool_size; i++) {
            Page page = new Page();
            pages.add(page);
        }
        this.diskManager = diskManager;
    }

    public int getPoolSize() {
        return poolSize;
    }

    public static void MarkPageDirty(Page page) {
        page.dirty = true;
    }

    /**
     * 从缓冲池中获取指定位置的页面。如果页面已存在于缓冲池中，则增加其引脚计数并返回该页面。
     * 如果页面不存在，则查找一个受害者页面进行替换，更新页面内容并从磁盘读取新页面。
     *
     * @param position 页面在磁盘上的位置，包括文件名和偏移量
     * @return 返回请求的页面，如果没有可用页面则返回 null
     * @throws DBException 如果在获取页面过程中发生数据库异常
     */
    public Page FetchPage(PagePosition position) throws DBException {
        if (pageMap.containsKey(position)) {
            Integer frame_id = pageMap.get(position);
            Page page = pages.get(frame_id);
            page.pin_count++;
            if (page.pin_count == 1) {
                replacer.Pin(frame_id);
            }
            return page;
        } else {
            int frame_id = find_victim_page();
            if (frame_id == -1) {
                return null;
            }
            Page page = pages.get(frame_id);
            update_page(page, position, frame_id);
            diskManager.ReadPage(page, position.filename, position.offset, Page.DEFAULT_PAGE_SIZE);
            page.pin_count++;
            if (page.pin_count == 1) {
                replacer.Pin(frame_id);
            }
            return page;
        }
    }

    /**
     * @description: 取消固定pin_count>0的在缓冲池中的page
     * @return {bool} 如果目标页的pin_count<=0则返回false，否则返回true
     * @param {position} position 目标page的position
     * @param {bool}     is_dirty 若目标page应该被标记为dirty则为true，否则为false
     */
    public boolean unpin_page(PagePosition position, boolean is_dirty) {
        Integer frame_id = pageMap.get(position);
        if (frame_id != null) {
            Page page = pages.get(frame_id);
            if (page.pin_count == 0) {
                return false;
            }
            page.pin_count--;
            if (page.pin_count == 0) {
                replacer.Unpin(frame_id);
            }
            page.dirty |= is_dirty;
            return true;
        } else {
            return false;
        }
    }

    /**
     * @description: 将目标页写回磁盘，不考虑当前页面是否正在被使用
     * @return {bool} 成功则返回true，否则返回false(只有page_table_中没有目标页时)
     * @param {PageId} page_id 目标页的page_id，不能为INVALID_PAGE_ID
     */
    public boolean FlushPage(PagePosition position) throws DBException {
        Integer frame_id = pageMap.get(position);
        if (frame_id != null) {
            Page page = pages.get(frame_id);
            diskManager.FlushPage(page);
            page.dirty = false;
            return true;
        } else {
            return false;
        }
    }

    /**
     * 创建一个新的页面并将其分配到缓冲池中。
     *
     * @param filename 要分配页面的文件名
     * @return 新创建的页面，如果没有可用的页面则返回 null
     * @throws DBException 如果在分配页面时发生错误
     */
    public Page NewPage(String filename) throws DBException {
        int frame_id = find_victim_page();
        if (frame_id == -1) {
            return null;
        }
        int new_page_offset = diskManager.AllocatePage(filename) * Page.DEFAULT_PAGE_SIZE;
        Page page = pages.get(frame_id);

        PagePosition position = new PagePosition(filename, new_page_offset);
        update_page(page, position, frame_id);

        diskManager.FlushPage(page);
        page.pin_count++;

        if (page.pin_count == 1) {
            replacer.Pin(frame_id);
        }
        return page;
    }

    /**
     * 从缓冲池中删除指定位置的页面。
     * 
     * @param position 要删除的页面的位置。
     * @return 如果成功删除页面则返回 true；如果页面被锁定或不存在则返回 false。
     * @throws DBException 如果在删除过程中发生数据库异常。
     */
    public boolean DeletePage(PagePosition position) throws DBException {
        Integer frame_id = pageMap.get(position);
        if (frame_id != null) {
            Page page = pages.get(frame_id);
            if (page.pin_count > 0) {
                return false;
            }
            if (page.dirty) {
                diskManager.FlushPage(page);
                page.dirty = false;
            }
            pages.remove(frame_id);
            pageMap.remove(position);
            freeList.add(frame_id);
            // pin count must be 0
            return true;
        } else {
            return false;
        }

    }

    /**
     * 将指定文件的所有页面刷新到磁盘。
     *
     * @param filename 要刷新的文件名
     * @throws DBException 如果在刷新过程中发生数据库异常
     */
    public void FlushAllPages(String filename) throws DBException {
        for (Map.Entry<PagePosition, Integer> entry : this.pageMap.entrySet()) {
            PagePosition position = entry.getKey();
            Integer frame_id = entry.getValue();
            if (filename.equals("") || position.filename.equals(filename)) {
                Page page = pages.get(frame_id);
                diskManager.FlushPage(page);
                page.dirty = false;
            }
        }
    }

    /**
     * 删除指定文件名的所有页面。
     *
     * @param filename 要删除页面的文件名
     * @throws DBException 如果在删除过程中发生数据库异常
     */
    public void DeleteAllPages(String filename) throws DBException {
        ArrayList<PagePosition> positions = new ArrayList<>();
        for (Map.Entry<PagePosition, Integer> entry : this.pageMap.entrySet()) {
            if (entry.getKey().filename.equals(filename)) {
                positions.add(entry.getKey());
            }
        }
        for (PagePosition position : positions) {
            DeletePage(position);
        }
    }

    /**
     * 查找一个受害者页面以进行替换。
     * 
     * 如果自由列表不为空，则从中移除并返回一个页面ID。
     * 否则，使用LRU or Clock替换算法选择一个页面ID。如果选择的页面是脏页，
     * 则将其刷新到磁盘。返回找到的页面ID。
     * 
     * @return 被替换的页面ID，如果没有可替换的页面则返回-1。
     * @throws DBException 如果在查找或刷新页面时发生错误。
     */
    private int find_victim_page() throws DBException {
        if (!freeList.isEmpty()) {
            return freeList.removeFirst();
        } else {
            int frame_id = replacer.Victim();
            if (frame_id != -1) {
                Page page = pages.get(frame_id);
                if (page.dirty) {
                    diskManager.FlushPage(page);
                }
            }
            return frame_id;
        }
    }

    /**
     * 更新指定页面的位置和状态。如果页面是脏页，则将其刷新到磁盘。
     * 
     * @param page         要更新的页面对象
     * @param new_position 新的位置
     * @param frame_id     帧的标识符
     * @throws DBException 如果在更新过程中发生数据库异常
     */
    private void update_page(Page page, PagePosition new_position, int frame_id) throws DBException {
        if (page.dirty) {
            diskManager.FlushPage(page);
        }
        // remove old one
        pageMap.remove(page.position);
        // add new one
        pageMap.put(new_position, frame_id);
        Arrays.fill(page.data.array(), (byte) 0);

        page.position = new_position;
        page.pin_count = 0;
        page.dirty = false;
    }
}
