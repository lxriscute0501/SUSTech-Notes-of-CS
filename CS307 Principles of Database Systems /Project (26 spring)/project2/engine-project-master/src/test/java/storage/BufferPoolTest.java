package storage;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.storage.BufferPool;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.storage.Page;
import edu.sustech.cs307.storage.PagePosition;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.lang.reflect.Field;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.IntStream;

import static org.assertj.core.api.Assertions.*;

class BufferPoolTest {
    private BufferPool bufferPool;
    private DiskManager diskManager;
    private Map<String, Integer> fileOffsets;

    @BeforeEach
    void setUp(@TempDir Path tempDir) throws DBException {
        fileOffsets = new HashMap<>();
        diskManager = new DiskManager(tempDir.toString(), fileOffsets);
        bufferPool = new BufferPool(3, diskManager); // 使用较小的缓冲池便于测试替换策略
        diskManager.CreateFile("test.db");
        for(int i = 0;i < 20;i ++) {
            Page page = bufferPool.NewPage("test.db");
            bufferPool.FlushPage(page.position);
            bufferPool.unpin_page(page.position, true);
        }
    }

    @Test
    @DisplayName("获取未缓存的页面应从磁盘加载")
    void testFetchPage_shouldLoadFromDiskWhenNotCached() throws DBException {
        PagePosition position = new PagePosition("test.db", 0);
        Page page = bufferPool.FetchPage(position);

        assertThat(page).isNotNull();
        assertThat(page.position).isEqualTo(position);
        assertThat(page.pin_count).isEqualTo(1);
    }

    @Test
    @DisplayName("取消固定页面应减少固定计数")
    void testUnpinPage_shouldDecreasePinCount() throws DBException {
        PagePosition position = new PagePosition("test.db", 0);
        Page page = bufferPool.FetchPage(position);

        boolean result = bufferPool.unpin_page(position, true);
        assertThat(result).isTrue();
        assertThat(page.pin_count).isEqualTo(0);
        assertThat(page.dirty).isTrue();
    }

    @Test
    @DisplayName("分配新页面应正确初始化")
    void testNewPage_shouldAllocateNewPage() throws DBException {
        Page newPage = bufferPool.NewPage("test.db");

        assertThat(newPage).isNotNull();
        assertThat(newPage.position.filename).isEqualTo("test.db");
        assertThat(newPage.pin_count).isEqualTo(1);
    }

    @Test
    @DisplayName("LRU替换策略应正确执行")
    void testLRUReplacement() throws DBException, IllegalAccessException, NoSuchFieldException {
        // 填充缓冲池
        bufferPool.FetchPage(new PagePosition("test.db", 0));
        bufferPool.FetchPage(new PagePosition("test.db", 4096));
        bufferPool.FetchPage(new PagePosition("test.db", 8192));

        // 此时全被都被pin住，无法使用
        Page nullPage = bufferPool.NewPage("test.db");
        assertThat(nullPage).isNull();

        var pos1 = new PagePosition("test.db", 0);
        var pos2 = new PagePosition("test.db", 0);
        assertThat(pos1.hashCode() == pos2.hashCode()).isTrue();
        assertThat(pos1.equals(pos2)).isTrue();

        bufferPool.unpin_page(new PagePosition("test.db", 0), true);

        // 有一个没有被pin住
        Page newPage = bufferPool.NewPage("test.db");
        assertThat(newPage).isNotNull();

        Class<?> clazz = BufferPool.class;
        Field field = clazz.getDeclaredField("pageMap");
        field.setAccessible(true);
        Map<PagePosition, Integer> pageMap = (Map<PagePosition, Integer>) field.get(bufferPool);
        assertThat(pageMap.size()).isEqualTo(3);
    }

    @Test
    @DisplayName("刷新页面应清除脏标志")
    void testFlushPage_shouldClearDirtyFlag() throws DBException {
        PagePosition position = new PagePosition("test.db", 0);
        diskManager.CreateFile("test.db");
        Page page = bufferPool.NewPage("test.db");
        bufferPool.FlushPage(page.position);
        {
            Page fetchPage = bufferPool.FetchPage(position);
            BufferPool.MarkPageDirty(fetchPage);

            boolean result = bufferPool.FlushPage(position);
            assertThat(result).isTrue();
            assertThat(fetchPage.dirty).isFalse();
        }
    }

    @Test
    @DisplayName("删除页面应从缓冲池移除")
    void testDeletePage_shouldRemoveFromBuffer() throws DBException, NoSuchFieldException, IllegalAccessException {
        PagePosition position = new PagePosition("test.db", 0);
        bufferPool.FetchPage(position);
        bufferPool.unpin_page(position, false);

        boolean result = bufferPool.DeletePage(position);
        assertThat(result).isTrue();
        Class<?> clazz = BufferPool.class;
        Field field = clazz.getDeclaredField("pageMap");
        field.setAccessible(true);
        Map<PagePosition, Integer> pageMap = (Map<PagePosition, Integer>) field.get(bufferPool);
        assertThat(pageMap.containsKey(position)).isFalse();
    }
    @Test
    @DisplayName("当缓冲池满且所有页面被固定时，新页面分配应失败")
    void testNewPageFailureWhenPoolFullAndAllPinned() throws DBException {
        // 填充缓冲池并保持所有页面被 pinned
        for (int i = 0;i < 3;i ++){
            bufferPool.FetchPage(new PagePosition("test.db", i * Page.DEFAULT_PAGE_SIZE));
        }

        Page newPage = bufferPool.NewPage("test.db");
        assertThat(newPage).isNull();
    }

    @Test
    @DisplayName("多次获取同一页面应增加固定计数")
    void testMultipleFetchIncrementsPinCount() throws DBException {
        PagePosition position = new PagePosition("test.db", 0);

        Page page1 = bufferPool.FetchPage(position);
        Page page2 = bufferPool.FetchPage(position);

        assertThat(page1).isSameAs(page2); // 确保是同一个对象
        assertThat(page1.pin_count).isEqualTo(2);
    }

    @Test
    @DisplayName("刷新不存在的页面应返回失败")
    void testFlushNonExistentPageShouldFail() throws DBException {
        PagePosition invalidPos = new PagePosition("invalid.db", 9999);

        assertThat(bufferPool.FlushPage(invalidPos)).isFalse();
    }

    @Test
    @DisplayName("删除未缓存的页面应返回失败")
    void testDeleteNonCachedPageShouldFail() throws DBException {
        PagePosition nonCachedPos = new PagePosition("test.db", 9999);

        boolean result = bufferPool.DeletePage(nonCachedPos);
        assertThat(result).isFalse();
    }

    @Test
    @DisplayName("多次取消固定页面不应导致计数为负")
    void testMultipleUnpinShouldNotMakeNegativeCount() throws DBException {
        PagePosition position = new PagePosition("test.db", 0);
        Page page = bufferPool.FetchPage(position);

        bufferPool.unpin_page(position, false); // pin_count 1 -> 0
        boolean result = bufferPool.unpin_page(position, false); // 尝试再次 unpin

        assertThat(result).isFalse();
        assertThat(page.pin_count).isEqualTo(0);
    }

    @Test
    @DisplayName("脏页面在替换时应自动刷新")
    void testDirtyPageAutoFlushOnEviction() throws Exception {
        // 1. 创建并标记脏页
        PagePosition targetPos = new PagePosition("test.db", 0);
        Page dirtyPage = bufferPool.FetchPage(targetPos);
        for(int i = 0;i < Page.DEFAULT_PAGE_SIZE;i ++) {
            dirtyPage.data.array()[i] = (byte) ((byte) i % 128);
        }
        byte[] data = Arrays.copyOf(dirtyPage.data.array(), Page.DEFAULT_PAGE_SIZE);
        BufferPool.MarkPageDirty(dirtyPage);
        bufferPool.unpin_page(targetPos, true);

        // 2. 强制触发页面替换
        for (int i = 0;i < 3;i ++) {
            PagePosition newPos = new PagePosition("test.db", 4096 * (i + 1));
            bufferPool.FetchPage(newPos);
            bufferPool.unpin_page(newPos, false);
        }

        Page page = new Page();

        // 3. 验证磁盘是否被更新
        diskManager.ReadPage(page, targetPos.filename, targetPos.offset, Page.DEFAULT_PAGE_SIZE);
        assertThat(page.data.array()).isEqualTo(data);
    }

    @Test
    @DisplayName("不同文件的页面应独立管理")
    void testDifferentFilePagesManagement() throws DBException {
        // 1. 创建第二个文件
        diskManager.CreateFile("test2.db");
        PagePosition pos1 = new PagePosition("test.db", 0);
        Page page = bufferPool.NewPage("test2.db");
        bufferPool.unpin_page(page.position, true);
        PagePosition pos2 = new PagePosition("test2.db", 0);

        // 2. 获取两个文件的页面
        Page page1 = bufferPool.FetchPage(pos1);
        Page page2 = bufferPool.FetchPage(pos2);

        // 3. 验证独立管理
        assertThat(page1).isNotNull();
        assertThat(page2).isNotNull();
        assertThat(page1.position.filename).isNotEqualTo(page2.position.filename);
    }
}
