package edu.sustech.cs307.record;

import io.netty.buffer.ByteBuf;

/**
 * 记录文件头部类，用于管理记录文件的元数据。
 * 包含记录大小、页面数量、每页记录数量、首个空闲页面和位图大小等信息。
 * 
 * <p>
 * 该类提供了对这些属性的获取和设置方法，所有数据均存储在 ByteBuf 中。
 * </p>
 */
public class RecordFileHeader {
    public final static int SIZE = 20;
    ByteBuf header;

    public RecordFileHeader(ByteBuf header) {
        this.header = header;
    }
    /**
     * 获取记录的大小。
     *
     * @return 记录的字节大小。
     */
    public int getRecordSize() {
        return header.getInt(0);
    }

    /**
     * 设置记录的大小。
     *
     * @param recordSize 记录的大小，以字节为单位。
     */
    public void setRecordSize(int recordSize) {
        header.setInt(0, recordSize);
    }

    /**
     * 获取记录文件头中页面的数量。
     *
     * @return 页面数量
     */
    public int getNumberOfPages() {
        return header.getInt(4);
    }

    /**
     * 设置记录文件头的页面数量。
     *
     * @param numberOfPages 要设置的页面数量
     */
    public void setNumberOfPages(int numberOfPages) {
        header.setInt(4, numberOfPages);
    }

    /**
     * 获取每页记录的数量。
     *
     * @return 每页记录的数量
     */
    public int getNumberOfRecordsPrePage() {
        return header.getInt(8);
    }

    /**
     * 设置每页记录的数量。
     *
     * @param numberOfRecordsPrePage 每页记录的数量
     */
    public void setNumberOfRecordsPrePage(int numberOfRecordsPrePage) {
        header.setInt(8, numberOfRecordsPrePage);
    }

    /**
     * 获取第一个空闲页面的索引。
     *
     * @return 第一个空闲页面的索引值。
     */
    public int getFirstFreePage() {
        return header.getInt(12);
    }

    /**
     * 设置记录文件头中的第一个空闲页面的索引。
     *
     * @param firstFreePage 第一个空闲页面的索引
     */
    public void setFirstFreePage(int firstFreePage) {
        header.setInt(12, firstFreePage);
    }

    /**
     * 获取位图大小。
     *
     * @return 位图的大小，以字节为单位。
     */
    public int getBitMapSize() {
        return header.getInt(16);
    }

    /**
     * 设置位图大小。
     *
     * @param bitMapSize 位图的大小，单位为字节。
     */
    public void setBitMapSize(int bitMapSize) {
        header.setInt(16, bitMapSize);
    }
}
