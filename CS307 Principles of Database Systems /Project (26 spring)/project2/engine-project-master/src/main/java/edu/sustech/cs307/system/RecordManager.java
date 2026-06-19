package edu.sustech.cs307.system;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import edu.sustech.cs307.record.RecordFileHandle;
import edu.sustech.cs307.record.RecordFileHeader;
import edu.sustech.cs307.record.RecordPageHeader;
import edu.sustech.cs307.storage.BufferPool;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.storage.Page;
import edu.sustech.cs307.storage.PagePosition;

/**
 * 记录管理器类，负责管理数据库记录的创建、删除和文件操作。
 * 
 * 该类提供了创建文件、删除文件、打开文件和关闭文件的方法，确保记录的大小在有效范围内，并处理与磁盘和缓冲池的交互。
 */
public class RecordManager {
    private final DiskManager diskManager;
    private final BufferPool bufferPool;
    private static final int MAX_RECORD_SIZE = 4000;

    public RecordManager(DiskManager diskManager, BufferPool bufferPool) {
        this.diskManager = diskManager;
        this.bufferPool = bufferPool;
    }

    /**
     * 创建一个新的文件并初始化其记录大小和页头信息。
     *
     * @param filename    文件名
     * @param record_size 记录大小，必须大于0且小于等于最大记录大小
     * @throws DBException 如果记录大小无效或文件创建失败
     */
    public void CreateFile(String filename, int record_size) throws DBException {
        if (record_size <= 0 || record_size > MAX_RECORD_SIZE) {
            throw new DBException(ExceptionTypes.InvalidTableWidth(record_size));
        }
        diskManager.CreateFile(filename);
        Page page = new Page();
        diskManager.ReadPage(page, filename, 0, Page.DEFAULT_PAGE_SIZE);
        RecordFileHeader recordFileHeader = new RecordFileHeader(page.data);
        recordFileHeader.setRecordSize(record_size);
        recordFileHeader.setNumberOfPages(0);
        recordFileHeader.setFirstFreePage(RecordPageHeader.NO_NEXT_FREE_PAGE);
        recordFileHeader.setNumberOfRecordsPrePage(
                (8 * (Page.DEFAULT_PAGE_SIZE - RecordPageHeader.SIZE) / (1 + record_size * 8)));
        recordFileHeader.setBitMapSize((recordFileHeader.getNumberOfRecordsPrePage() + 7) / 8);
        page.dirty = true;
        diskManager.FlushPage(page);
    }

    /**
     * 删除指定名称的文件。
     *
     * @param filename 要删除的文件名
     * @throws DBException 如果删除文件过程中发生错误
     */
    public void DeleteFile(String filename) throws DBException {
        diskManager.DeleteFile(filename);
    }

    /**
     * 打开指定名称的记录文件，并返回对应的记录文件句柄。
     *
     * @param table_name 要打开的记录文件的名称
     * @return 返回与指定记录文件关联的 RecordFileHandle
     * @throws DBException 如果在打开文件过程中发生错误
     */
    public RecordFileHandle OpenFile(String table_name) throws DBException {
        String data_file = String.format("%s/%s", table_name, "data");
        Page page = bufferPool.FetchPage(new PagePosition(data_file, 0));
        RecordFileHeader recordFileHeader = new RecordFileHeader(page.data);
        return new RecordFileHandle(diskManager, bufferPool, data_file, recordFileHeader);
    }

    /**
     * 关闭指定的记录文件，并将所有页面刷新到磁盘。
     *
     * @param recordFileHandle 要关闭的记录文件句柄
     * @throws DBException 如果在关闭文件时发生数据库异常
     */
    public void CloseFile(RecordFileHandle recordFileHandle) throws DBException {
        bufferPool.FlushAllPages(recordFileHandle.getFilename());
    }
}
