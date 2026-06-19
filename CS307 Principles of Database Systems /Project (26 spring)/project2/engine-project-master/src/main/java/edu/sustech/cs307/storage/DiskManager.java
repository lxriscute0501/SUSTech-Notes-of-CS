package edu.sustech.cs307.storage;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import edu.sustech.cs307.DBEntry;
import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;

import org.pmw.tinylog.Logger;

/**
 * 磁盘管理器类，用于管理文件的读写和元数据的存储。
 * 提供创建文件、读取页面、写入页面、分配页面和删除文件等功能。
 * 
 * <p>
 * 该类支持从磁盘读取和写入文件，并维护文件的页面信息。
 * </p>
 */
public class DiskManager {
    private final String currentDir;
    public Map<String, Integer> filePages;

    private static final String DISK_MANAGER_META = "disk_manager_meta.json";

    public static Map<String, Integer> read_disk_manager_meta() throws DBException {
        Path path = Path.of(String.format("%s/%s", DBEntry.DB_NAME, DISK_MANAGER_META));
        // read the meta file
        File META_FILE = new File(path.toString());
        if (!META_FILE.exists()) {
            Logger.info("File does not exist, creating a new one...");
            return Map.of();
        }
        try (Reader reader = new FileReader(META_FILE)) {
            TypeReference<Map<String, Integer>> typeRef = new TypeReference<>() {
            };
            ObjectMapper objectMapper = new ObjectMapper();
            Map<String, Integer> loadedTables = objectMapper.readValue(reader, typeRef);
            if (loadedTables != null) {
                return new HashMap<String, Integer>(loadedTables);
            } else {
                throw new DBException(ExceptionTypes.UnableLoadMetadata("Failed to load metadata"));
            }
        } catch (Exception e) {
            throw new DBException(ExceptionTypes.UnableLoadMetadata(e.getMessage()));
        }
    }

    /**
     * 将 DiskManager 的元数据转储到指定的元文件中。
     *
     * @param disk_manager 要转储元数据的 DiskManager 实例
     * @throws DBException 如果在写入元数据时发生错误
     */

    public static void dump_disk_manager_meta(DiskManager disk_manager) throws DBException {
        Map<String, Integer> filePages = disk_manager.filePages;
        Path path = Path.of(String.format("%s/%s", DBEntry.DB_NAME, DISK_MANAGER_META));
        // write the meta file
        File META_FILE = new File(path.toString());
        try (Writer writer = new FileWriter(META_FILE)) {
            ObjectMapper objectMapper = new ObjectMapper();
            objectMapper.writeValue(writer, filePages);
        } catch (Exception e) {
            throw new DBException(ExceptionTypes.UnableLoadMetadata(e.getMessage()));
        }
    }

    public DiskManager(String path, Map<String, Integer> filePages) {
        this.currentDir = path;
        this.filePages = filePages;
    }

    public String getCurrentDir() {
        return currentDir;
    }

    /**
     * 从指定文件中读取一个页面的数据到给定的 Page 对象中。
     *
     * @param page     要读取数据的 Page 对象，数据将被填充到 page.data 中。
     * @param filename 要读取的文件名。
     * @param offset   从文件中读取的起始偏移量。
     * @param length   要读取的数据长度。
     * @throws DBException 如果在读取过程中发生 I/O 错误或偏移量超出范围。
     */
    public void ReadPage(Page page, String filename, int offset, long length) throws DBException {
        // 拼接实际文件路径
        String real_path = currentDir + "/" + filename;
        try (FileInputStream fis = new FileInputStream(real_path)) {
            long seeked = fis.skip(offset);
            if (seeked < offset) {
                throw new DBException(ExceptionTypes.BadIOError(
                        String.format("Seek out of range, offset: %d, length: %d", seeked, length)));
            }
            // 直接将文件中一个页面大小的数据读取到 page.data 中
            if (fis.read(page.data.array()) != Page.DEFAULT_PAGE_SIZE) {
                // throw new DBException(ExceptionTypes.BadIOError(
                // String.format("Bad file at offset: %d, length: %d", seeked, length)));
            }
            page.position.offset = offset;
            page.position.filename = filename;
        } catch (IOException e) {
            throw new DBException(ExceptionTypes.BadIOError(e.getMessage()));
        }
    }

    /**
     * 将指定页面的数据刷新到磁盘。
     *
     * @param page 要刷新的页面对象，包含数据和位置信息。
     * @throws DBException 如果在写入过程中发生输入输出错误。
     */
    public void FlushPage(Page page) throws DBException {
        // 拼接实际文件路径
        String real_path = currentDir + "/" + page.position.filename;

        // 使用 RandomAccessFile 和 FileChannel 来定位并写入数据
        try (RandomAccessFile raf = new RandomAccessFile(real_path, "rw");
                FileChannel channel = raf.getChannel()) {
            // 定位到对应的文件偏移位置
            channel.position(page.position.offset);
            // 使用 ByteBuffer.wrap 避免额外的数据拷贝
            ByteBuffer buffer = ByteBuffer.wrap(page.data.array());
            while (buffer.hasRemaining()) {
                channel.write(buffer);
            }
            // 强制刷新到磁盘
            channel.force(true);
        } catch (IOException e) {
            throw new DBException(ExceptionTypes.BadIOError(e.getMessage()));
        }
    }

    public long GetFileSize(String filename) {
        String real_path = currentDir + "/" + filename;
        File file = new File(real_path);
        return file.length();
    }

    /**
     * 创建一个新文件。
     * 
     * @param filename 文件名
     * @throws DBException 如果文件创建失败或发生IO异常
     * 
     *                     该方法会检查指定路径下是否已存在同名文件。如果不存在，则会尝试创建该文件及其上级目录。
     *                     如果创建过程中发生任何异常，将抛出DBException。
     */
    public void CreateFile(String filename) throws DBException {
        String real_path = currentDir + "/" + filename;
        File file = new File(real_path);
        if (!file.exists()) {
            try {
                // 如果上级目录不存在则创建
                File parent = file.getParentFile();
                if (parent != null && !parent.exists()) {
                    parent.mkdirs();
                }
                if (!file.createNewFile()) {
                    throw new DBException(ExceptionTypes.BadIOError("File creation failed: " + real_path));
                }
                this.filePages.put(filename, 1);
            } catch (IOException e) {
                throw new DBException(ExceptionTypes.BadIOError(e.getMessage()));
            }
        }
    }

    // return file start;
    public Integer AllocatePage(String filename) throws DBException {
        Integer offset = this.filePages.get(filename);
        if (offset == null) {
            throw new DBException(ExceptionTypes.BadIOError(String.format("File not exists, %s", filename)));
        }
        this.filePages.put(filename, offset + 1);
        return offset;
    }

    public void DeleteFile(String filename) throws DBException {
        String real_path = currentDir + "/" + filename;
        File file = new File(real_path);
        if (file.exists()) {
            if (!file.delete()) {
                throw new DBException(ExceptionTypes.BadIOError("File deletion failed: " + real_path));
            }
            this.filePages.remove(filename);
        }
    }
}
