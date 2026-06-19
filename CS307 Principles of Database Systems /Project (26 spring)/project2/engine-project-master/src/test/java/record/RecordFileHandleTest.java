package record;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.record.*;
import edu.sustech.cs307.storage.BufferPool;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.storage.Page;
import edu.sustech.cs307.storage.PagePosition;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import org.junit.jupiter.api.*;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.nio.file.Files;
import java.util.UUID;
import java.io.IOException;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;

import static org.assertj.core.api.Assertions.*;

class RecordFileHandleTest {

    @TempDir
    static Path tempDir;

    static DiskManager diskManager;
    static BufferPool bufferPool;
    static RecordFileHandle fileHandle;
    static final String TEST_FILENAME = "test_file.db";

    @BeforeEach
    void setup() throws DBException, IOException {
        String randomDirName = "test-" + UUID.randomUUID().toString();
        tempDir = Files.createTempDirectory(randomDirName);

        Map<String, Integer> fileMap = new HashMap<>();
        diskManager = new DiskManager(tempDir.toString(), fileMap);
        bufferPool = new BufferPool(10, diskManager);

        // 创建测试文件并初始化文件头
        diskManager.CreateFile(TEST_FILENAME);
        ByteBuf headerData = Unpooled.buffer(Page.DEFAULT_PAGE_SIZE);

        headerData.writeInt(128);     // record size
        headerData.writeInt(1);       // initial pages
        headerData.writeInt(31);      // records per page
        headerData.writeInt(RecordPageHeader.NO_NEXT_FREE_PAGE);      // first free page
        headerData.writeInt(4);       // bitmap size


        Page headerPage = new Page();
        headerPage.data = headerData;
        headerPage.position = new PagePosition(TEST_FILENAME, 0);
        diskManager.FlushPage(headerPage);

        RecordFileHeader header = new RecordFileHeader(headerData);
        fileHandle = new RecordFileHandle(diskManager, bufferPool, TEST_FILENAME, header);
    }

    @Nested
    @DisplayName("基本CRUD操作测试")
    class BasicCRUDTests {
        @Test
        @DisplayName("插入并正确读取记录")
        void insertAndRetrieveRecord() throws DBException {
            ByteBuf data = Unpooled.buffer(128).writeBytes("TestData".getBytes());
            RID rid = fileHandle.InsertRecord(data);

            assertThat(fileHandle.GetRecord(rid).Serialize().array())
                    .startsWith("TestData".getBytes());
        }

        @Test
        @DisplayName("更新记录后数据持久化")
        void updateRecordPersistsChanges() throws DBException {
            RID rid = fileHandle.InsertRecord(Unpooled.buffer(128).writeBytes("Original".getBytes()));
            fileHandle.UpdateRecord(rid, Unpooled.buffer(128).writeBytes("Updated".getBytes()));

            assertThat(fileHandle.GetRecord(rid).Serialize().array())
                    .startsWith("Updated".getBytes());
        }

        @Test
        @DisplayName("删除记录后标记为无效")
        void deleteRecordMarksInvalid() throws DBException {
            RID rid = fileHandle.InsertRecord(Unpooled.buffer(128));
            fileHandle.DeleteRecord(rid);

            assertThat(fileHandle.IsRecord(rid)).isFalse();
        }
    }

    @Nested
    @DisplayName("异常处理测试")
    class ExceptionHandlingTests {
        @Test
        @DisplayName("访问无效页面应抛出异常")
        void accessInvalidPageThrowsException() {
            RID invalidRid = new RID(999, 0);

            assertThatThrownBy(() -> fileHandle.GetRecord(invalidRid))
                    .hasMessageContaining("out of range");
        }
    }

    @Nested
    @DisplayName("页面管理测试")
    class PageManagementTests {
        @Test
        @DisplayName("当页满时自动分配新页面")
        void autoAllocateNewPageWhenFull() throws DBException {
            int recordsPerPage = fileHandle.getFileHeader().getNumberOfRecordsPrePage();

            // Fill first page
            for (int i = 0; i < recordsPerPage; i++) {
                fileHandle.InsertRecord(Unpooled.buffer(128));
            }

            RID newRid = fileHandle.InsertRecord(Unpooled.buffer(128));
            assertThat(newRid.pageNum).isEqualTo(1);
        }

        @Test
        @DisplayName("释放页面后重用空间")
        void reuseFreedPageSpace() throws DBException {
            RID rid1 = fileHandle.InsertRecord(Unpooled.buffer(128));
            fileHandle.DeleteRecord(rid1);

            RID rid2 = fileHandle.InsertRecord(Unpooled.buffer(128));
            assertThat(rid2.pageNum).isEqualTo(rid1.pageNum);
        }
    }

    @Nested
    @DisplayName("元数据一致性测试")
    class MetadataConsistencyTests {
        @Test
        @DisplayName("文件头信息正确更新")
        void headerUpdatesCorrectly() throws DBException {
            int initialPages = fileHandle.getFileHeader().getNumberOfPages();

            fileHandle.InsertRecord(Unpooled.buffer(128));
            assertThat(fileHandle.getFileHeader().getNumberOfPages())
                    .isEqualTo(initialPages + 1);

            // Force new page allocation
            int recordsPerPage = fileHandle.getFileHeader().getNumberOfRecordsPrePage();
            for (int i = 0; i < recordsPerPage; i++) {
                fileHandle.InsertRecord(Unpooled.buffer(128));
            }
            assertThat(fileHandle.getFileHeader().getNumberOfPages())
                    .isEqualTo(initialPages + 2);
        }

        @Test
        @DisplayName("位图状态正确反映记录状态")
        void bitmapAccuratelyReflectsState() throws DBException {
            RID rid = fileHandle.InsertRecord(Unpooled.buffer(128));
            RecordPageHandle handle = fileHandle.FetchPageHandle(rid.pageNum);

            assertThat(BitMap.isSet(handle.bitmap, rid.slotNum)).isTrue();
            fileHandle.DeleteRecord(rid);
            assertThat(BitMap.isSet(handle.bitmap, rid.slotNum)).isFalse();
        }
    }

    @Nested
    @DisplayName("持久化测试")
    class PersistenceTests {
        @Test
        @DisplayName("数据跨会话持久化")
        void dataPersistsAcrossSessions() throws DBException {
            ByteBuf data = Unpooled.buffer(128).writeBytes("PersistentData".getBytes());
            RID rid = fileHandle.InsertRecord(data);

            // 强制刷新并重新初始化
            bufferPool.FlushAllPages(TEST_FILENAME);
            BufferPool newPool = new BufferPool(10, diskManager);
            RecordFileHandle newHandle = new RecordFileHandle(diskManager, newPool, TEST_FILENAME, fileHandle.getFileHeader());

            assertThat(newHandle.IsRecord(rid)).isTrue();
        }

        @Test
        @DisplayName("脏页写入磁盘机制")
        void autoFlushDirtyPages() throws DBException {
            RID rid = fileHandle.InsertRecord(Unpooled.buffer(128));
            bufferPool.unpin_page(new PagePosition(TEST_FILENAME, rid.pageNum * Page.DEFAULT_PAGE_SIZE), true);
            bufferPool.FlushAllPages(TEST_FILENAME);

            // 通过新缓冲池验证
            BufferPool newPool = new BufferPool(10, diskManager);
            RecordFileHandle newHandle = new RecordFileHandle(diskManager, newPool, TEST_FILENAME, fileHandle.getFileHeader());
            assertThat(newHandle.IsRecord(rid)).isTrue();
        }
    }

    // [...] 保留原有setup代码
}
