package edu.sustech.cs307.record;

import io.netty.buffer.ByteBuf;

/**
 * 记录页面头部类，用于管理记录页面的元数据。
 * 包含有关下一个空闲页面编号和记录数量的信息。
 */
public class RecordPageHeader {
    static public int SIZE = 8;
    static public final int NO_NEXT_FREE_PAGE = 0x7fffffff;

    public ByteBuf data;

    public int getNextFreePageNo() {
        return data.getInt(0);
    }

    public void setNextFreePageNo(int pageNo) {
        data.setInt(0, pageNo);
    }

    public int getNumberOfRecords() {
        return data.getInt(4);
    }

    public void setNumberOfRecords(int num) {
        data.setInt(4, num);
    }

    public RecordPageHeader(ByteBuf data) {
        this.data = data;
    }
}
