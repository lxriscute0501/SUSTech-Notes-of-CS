package record;

import edu.sustech.cs307.record.BitMap;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import java.util.Arrays;
import static org.assertj.core.api.Assertions.*;

class BitMapTest {
    @Test
    @DisplayName("初始化位图")
    public void testInit() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        for (int i = 0; i < 16; i++) {
            assertThat(bm.getByte(i)).isEqualTo((byte)0);
        }
    }

    @Test
    @DisplayName("设置位图位")
    public void testSet() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        BitMap.set(bm, 5);
        assertThat(BitMap.isSet(bm, 5)).isTrue();
        for (int i = 0; i < 16 * 8; i++) {
            if (i == 5) {
                assertThat(BitMap.isSet(bm, i)).isTrue();
            } else {
                assertThat(BitMap.isSet(bm, i)).isFalse();
            }
        }
    }

    @Test
    @DisplayName("重置位图位")
    public void testReset() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        BitMap.set(bm, 5);
        BitMap.reset(bm, 5);
        assertThat(BitMap.isSet(bm, 5)).isFalse();
        for (int i = 0; i < 16 * 8; i++) {
            assertThat(BitMap.isSet(bm, i)).isFalse();
        }
    }

    @Test
    @DisplayName("检查位图位")
    public void testIsSet() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        assertThat(BitMap.isSet(bm, 5)).isFalse();
        BitMap.set(bm, 5);
        assertThat(BitMap.isSet(bm, 5)).isTrue();
    }

    @Test
    @DisplayName("查找下一个位图位")
    public void testNextBit() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        BitMap.set(bm, 5);
        assertThat(BitMap.nextBit(true, bm, 16 * 8, 0)).isEqualTo(5);
        assertThat(BitMap.nextBit(true, bm, 16 * 8, 5)).isEqualTo(16 * 8);
        assertThat(BitMap.nextBit(false, bm, 16 * 8, 0)).isEqualTo(1);
        assertThat(BitMap.nextBit(false, bm, 16 * 8, 5)).isEqualTo(6);
    }

    @Test
    @DisplayName("查找第一个位图位")
    public void testFirstBit() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        BitMap.set(bm, 5);
        assertThat(BitMap.firstBit(true, bm, 16 * 8)).isEqualTo(5);
        assertThat(BitMap.firstBit(false, bm, 16 * 8)).isEqualTo(0);
    }

    @Test
    @DisplayName("边界测试")
    public void testBoundary() {
        ByteBuf bm = Unpooled.buffer(16);
        BitMap.init(bm);
        BitMap.set(bm, 0);
        assertThat(BitMap.isSet(bm, 0)).isTrue();
        BitMap.set(bm, 16 * 8 - 1);
        assertThat(BitMap.isSet(bm, 16 * 8 - 1)).isTrue();
    }

}
