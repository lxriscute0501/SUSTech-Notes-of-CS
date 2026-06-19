package edu.sustech.cs307.record;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;

/**
 * 记录类，用于管理字节缓冲区数据及其大小。
 * 提供多种构造函数以支持不同的初始化方式。
 * 支持数据的序列化和反序列化操作。
 */
public class Record {
    // 字节缓冲区数据
    ByteBuf data;
    // 数据大小
    int size;

    /**
     * 使用指定的字节缓冲区和大小初始化记录。
     *
     * @param data 字节缓冲区
     * @param size 数据大小
     */
    public Record(ByteBuf data, int size) {
        this.data = data;
        this.size = size;
    }

    /**
     * 默认构造函数，初始化为空记录。
     */
    public Record() {
        this.data = null;
        this.size = 0;
    }

    /**
     * 复制构造函数，基于现有记录创建新记录。
     *
     * @param record 现有记录
     */
    public Record(Record record) {
        this.data = Unpooled.buffer(record.data.capacity()).writeBytes(record.data);
        this.size = record.size;
    }

    /**
     * 使用指定大小初始化记录。
     *
     * @param size 数据大小
     */
    public Record(int size) {
        this.data = Unpooled.buffer(size);
        this.size = size;
    }

    /**
     * 使用字节缓冲区初始化记录。
     *
     * @param data 字节缓冲区
     */
    public Record(ByteBuf data) {
        this.data = data.copy();
        this.size = data.capacity();
    }

    /**
     * 设置记录的数据。
     *
     * @param data 新的字节缓冲区数据
     */
    public void SetData(ByteBuf data) {
        this.data = data.copy();
    }

    /**
     * 从字节缓冲区反序列化数据。
     *
     * @param data 字节缓冲区
     */
    public void Deserialize(ByteBuf data) {
        this.data = data.copy();
        this.size = data.capacity();
    }

    /**
     * 序列化记录数据为字节缓冲区。
     *
     * @return 序列化后的字节缓冲区
     */
    public ByteBuf Serialize() {
        return this.data.copy();
    }

    /**
     * 获取指定偏移量和长度的列值。
     *
     * @param offset 偏移量
     * @param len    长度
     * @return 指定范围的字节缓冲区
     */
    public ByteBuf GetColumnValue(int offset, int len) {
        return this.data.slice(offset, len);
    }

    /**
     * 设置指定偏移量的列值。
     *
     * @param offset      偏移量
     * @param columnValue 列值字节数组
     */
    public void SetColumnValue(int offset, byte[] columnValue) {
        this.data.writerIndex(offset).writeBytes(columnValue);
        this.data.resetWriterIndex();
    }

    /**
     * 获取只读数据的字节缓冲区。
     *
     * @return 只读字节缓冲区
     */
    public ByteBuf getReadOnlyData() {
        return this.data.asReadOnly();
    }
}
