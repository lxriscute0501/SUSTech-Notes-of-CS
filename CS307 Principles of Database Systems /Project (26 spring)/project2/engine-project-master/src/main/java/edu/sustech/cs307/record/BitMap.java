package edu.sustech.cs307.record;

import io.netty.buffer.ByteBuf;

/**
 * BitMap 类用于管理位图的操作，包括初始化、设置、重置和查询位。
 * 位图使用字节数组表示，每个字节包含多个位。
 */
public class BitMap {
    private static final int BITMAP_WIDTH = 8;
    private static final int BITMAP_HIGHEST_BIT = 0x80;

    // 从地址bm开始的size个字节全部置0
    public static void init(ByteBuf bm) {
        bm.setZero(0, bm.capacity());
    }

    // pos位 置1
    public static void set(ByteBuf bm, int pos) {
        int bucket = getBucket(pos);
        byte bit = getBit(pos);
        bm.setByte(bucket, (byte) (bm.getByte(bucket) | bit));
    }

    /**
     * 重置指定位置的位图，将对应位置的位设置为0。
     *
     * @param bm  需要操作的 ByteBuf 对象
     * @param pos 要重置的位的位置
     */
    public static void reset(ByteBuf bm, int pos) {
        int bucket = getBucket(pos);
        byte bit = getBit(pos);
        bm.setByte(bucket, (byte) (bm.getByte(bucket) & ~bit));
    }

    /**
     * 检查位图中指定位置的位是否被设置。
     *
     * @param bm  需要检查的字节缓冲区
     * @param pos 位图中要检查的位置
     * @return 如果指定位置的位被设置，则返回 true；否则返回 false
     */
    public static boolean isSet(ByteBuf bm, int pos) {
        int bucket = getBucket(pos);
        byte bit = getBit(pos);
        return (bm.getByte(bucket) & bit) != 0;
    }

    /**
     * 查找在位图中下一个与给定值相同的位。
     *
     * @param bit  要查找的位值（true 或 false）。
     * @param bm   位图的 ByteBuf 对象。
     * @param maxN 位图的最大索引。
     * @param curr 当前索引，从下一个索引开始查找。
     * @return 返回下一个与给定位值相同的索引，如果未找到则返回 maxN。
     */
    public static int nextBit(boolean bit, ByteBuf bm, int maxN, int curr) {
        for (int i = curr + 1; i < maxN; i++) {
            if (isSet(bm, i) == bit) {
                return i;
            }
        }
        return maxN;
    }

    /**
     * 返回在位图中第一个匹配的位的索引。
     *
     * @param bit  要查找的位值（true 或 false）。
     * @param bm   位图的 ByteBuf 对象。
     * @param maxN 搜索的最大索引值。
     * @return 第一个匹配的位的索引，如果未找到则返回 -1。
     */
    public static int firstBit(boolean bit, ByteBuf bm, int maxN) {
        return nextBit(bit, bm, maxN, -1);
    }

    /**
     * 根据给定的位置计算对应的桶索引。
     *
     * @param pos 位置值
     * @return 对应的桶索引
     */
    private static int getBucket(int pos) {
        return pos / BITMAP_WIDTH;
    }

    /**
     * 获取指定位置的位图中的位值。
     *
     * @param pos 位图中的位置，必须为非负整数。
     * @return 指定位置的位值，返回值为1表示该位置为1，返回值为0表示该位置为0。
     */
    private static byte getBit(int pos) {
        return (byte) (BITMAP_HIGHEST_BIT >>> (pos % BITMAP_WIDTH));
    }
}
