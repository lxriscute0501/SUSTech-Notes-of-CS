package storage;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.storage.Page;
import org.junit.jupiter.api.*;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import static org.assertj.core.api.Assertions.assertThat;
import static org.junit.jupiter.api.Assertions.assertThrows;

class DiskManagerTest {
    @TempDir
    static Path tempDir;
    private DiskManager diskManager;
    private static final String TEST_FILE = "test_file.dat";
    private static final String NON_EXISTENT_FILE = "no_such_file.dat";
    private static final int PAGE_SIZE = Page.DEFAULT_PAGE_SIZE;

    @BeforeEach
    void setUp() throws IOException {
        String randomDirName = "test-" + UUID.randomUUID().toString();
        tempDir = Files.createTempDirectory(randomDirName);

        Map<String, Integer> map = new HashMap<>();
        diskManager = new DiskManager(tempDir.toString(), map);
    }

    @Test
    @DisplayName("创建新文件应成功")
    void createNewFile() throws Exception {
        diskManager.CreateFile(TEST_FILE);

        Path filePath = tempDir.resolve(TEST_FILE);
        assertThat(filePath)
                .exists()
                .isRegularFile();
    }

    @Test
    @DisplayName("创建嵌套目录文件")
    void createNestedFile() throws Exception {
        String nestedFile = "nested/dir/test_file.dat";
        diskManager.CreateFile(nestedFile);

        Path filePath = tempDir.resolve(nestedFile);
        assertThat(filePath)
                .exists()
                .isRegularFile();
    }

    @Test
    @DisplayName("获取文件大小")
    void getFileSize() throws Exception {
        // 创建测试文件并写入数据
        byte[] data = new byte[1024];
        Path filePath = tempDir.resolve(TEST_FILE);
        Files.write(filePath, data);

        long size = diskManager.GetFileSize(TEST_FILE);
        assertThat(size).isEqualTo(1024);
    }

    @Test
    @DisplayName("读取有效页")
    void readValidPage() throws Exception {
        // 准备测试数据
        byte[] expected = new byte[PAGE_SIZE];
        for (int i = 0; i < PAGE_SIZE; i++) {
            expected[i] = (byte) (i % 256);
        }
        Path filePath = tempDir.resolve(TEST_FILE);
        Files.write(filePath, expected);

        // 执行读取
        Page page = new Page();
        diskManager.ReadPage(page, TEST_FILE, 0, PAGE_SIZE);

        assertThat(page.data.array())
                .containsExactly(expected);
        assertThat(page.position.filename).isEqualTo(TEST_FILE);
        assertThat(page.position.offset).isZero();
    }

    @Test
    @DisplayName("写入并刷新页")
    void flushPage() throws Exception {
        // 准备测试页
        Page page = new Page();
        for (int i = 0; i < PAGE_SIZE; i++) {
            page.data.array()[i] = (byte) (i % 128);
        }
        page.dirty = true;
        page.position.filename = TEST_FILE;
        page.position.offset = 0;

        // 写入文件
        diskManager.CreateFile(TEST_FILE);
        diskManager.FlushPage(page);

        // 验证写入内容
        Path filePath = tempDir.resolve(TEST_FILE);
        byte[] fileContent = Files.readAllBytes(filePath);
        assertThat(fileContent)
                .hasSize(PAGE_SIZE)
                .containsExactly(page.data.array());
    }

    @Test
    @DisplayName("部分写入测试")
    void partialWriteTest() throws Exception {
        // 准备超出单页的数据
        byte[] largeData = new byte[PAGE_SIZE * 2];
        for (int i = 0; i < largeData.length; i++) {
            largeData[i] = (byte) (i % 256);
        }

        // 写入两个页
        diskManager.CreateFile(TEST_FILE);

        Page page1 = new Page();
        System.arraycopy(largeData, 0, page1.data.array(), 0, PAGE_SIZE);
        page1.position.filename = TEST_FILE;
        page1.position.offset = 0;
        page1.dirty = true;
        diskManager.FlushPage(page1);

        Page page2 = new Page();
        System.arraycopy(largeData, PAGE_SIZE, page2.data.array(), 0, PAGE_SIZE);
        page2.position.filename = TEST_FILE;
        page2.position.offset = PAGE_SIZE;
        page2.dirty = true;
        diskManager.FlushPage(page2);

        // 验证完整数据
        Path filePath = tempDir.resolve(TEST_FILE);
        byte[] fileContent = Files.readAllBytes(filePath);
        assertThat(fileContent)
                .hasSize(PAGE_SIZE * 2)
                .containsExactly(largeData);
    }
}