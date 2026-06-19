package storage;

import edu.sustech.cs307.storage.replacer.LRUReplacer;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.stream.IntStream;

import static org.assertj.core.api.Assertions.*;

public class LRUReplacerTest {

    private LRUReplacer replacer;

    @BeforeEach
    void setUp() {
        replacer = new LRUReplacer(5);

    }

    @Test
    @DisplayName("空LRU测试")
    void testVictimWhenEmpty() {
        int victim = replacer.Victim();
        assertThat(victim).as("Victim should return -1 when LRUList is empty").isEqualTo(-1);
    }


    @Test
    @DisplayName("测试pin页面")
    void testPinFrame() {
        replacer.Pin(1);
        replacer.Pin(2);
        replacer.Pin(3);

        assertThat(replacer.size()).isEqualTo(3);


        replacer.Pin(4);
        replacer.Pin(5);
        assertThat(replacer.size()).isEqualTo(5);

        assertThatThrownBy(() -> replacer.Pin(6))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("REPLACER IS FULL");
    }

    @Test
    @DisplayName("测试反复pin页面")
    void testPinExistingInLRU() {
        replacer.Pin(1);

        replacer.Unpin(1);

        replacer.Pin(1);

        int victim = replacer.Victim();

        assertThat(victim).isEqualTo(-1);
        assertThat(replacer.size()).isEqualTo(1);

    }

    @Test
    @DisplayName("测试 Unpin 操作")
    void testUnpinFrame() {
        replacer.Pin(1);
        replacer.Pin(2);

        replacer.Unpin(1);
        assertThat(replacer.size()).isEqualTo(2);

        assertThat(replacer.Victim()).isEqualTo(1);

        assertThatThrownBy(() -> replacer.Unpin(1))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("UNPIN PAGE NOT FOUND");

        replacer.Unpin(2);
        assertThat(replacer.size()).isEqualTo(1);
    }

    @Test
    @DisplayName("测试 Victim 行为")
    void testVictimLRU() {
        replacer.Pin(1);
        replacer.Pin(2);
        replacer.Pin(3);

        replacer.Unpin(1); // LRUList = [1]
        replacer.Unpin(2); // LRUList = [1, 2]
        replacer.Unpin(3); // LRUList = [1, 2, 3]
        assertThat(replacer.size()).isEqualTo(3);

        int victim = replacer.Victim();
        assertThat(victim).isEqualTo(1);
        assertThat(replacer.size()).isEqualTo(2); // 还剩 [2, 3] + pinnedFrames(0)

        victim = replacer.Victim();
        assertThat(victim).isEqualTo(2);

        assertThat(replacer.size()).isEqualTo(1);
    }

    @Test
    @DisplayName("大规模数据测试")
    void testLargeScaleData() {
        int capacity = 100;
        replacer = new LRUReplacer(capacity);

        for (int i = 0; i < capacity; i++) {
            replacer.Pin(i);
        }
        assertThat(replacer.size()).as("All pinned, size should be 100").isEqualTo(capacity);

        for (int i = 0; i < capacity; i++) {
            replacer.Unpin(i);
        }
        assertThat(replacer.size()).isEqualTo(capacity);

        for (int i = 0; i < capacity / 2; i++) {
            replacer.Victim();
        }
        assertThat(replacer.size()).isEqualTo(capacity / 2);

        for (int i = capacity; i < capacity + 10; i++) {
            replacer.Pin(i);
        }

        assertThat(replacer.size()).isEqualTo(capacity / 2 + 10);
    }

    @Test
    @DisplayName("所有页面被固定时无法驱逐")
    void testAllPinnedNoVictim() {

        IntStream.range(0, 5).forEach(i -> replacer.Pin(i));

        assertThat(replacer.Victim()).isEqualTo(-1);

        replacer.Unpin(3);
        assertThat(replacer.Victim()).isEqualTo(3);
    }
    

    @Test
    @DisplayName("混合操作后的LRU顺序验证")
    void testComplexAccessPattern() {
        replacer.Pin(1);
        replacer.Pin(2);
        replacer.Unpin(1); // LRU: [1]
        replacer.Unpin(2); // LRU: [1, 2]

        replacer.Pin(1);   // 从LRU移除
        replacer.Unpin(1); // 重新加入末尾 → LRU: [2, 1]

        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(1);
    }

    @Test
    @DisplayName("容量满时固定新页面应失败")
    void testPinBeyondCapacity() {

        IntStream.range(0, 5).forEach(i -> {
            replacer.Pin(i);
            replacer.Unpin(i);
        });

        assertThatThrownBy(() -> replacer.Pin(5))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("REPLACER IS FULL");
    }

    @Test
    @DisplayName("重复固定同一页面不应改变状态")
    void testDuplicatePin() {
        replacer.Pin(1);
        replacer.Pin(1); // 重复固定

        replacer.Unpin(1);
        assertThat(replacer.Victim()).isEqualTo(1);
    }

    @Test
    @DisplayName("淘汰后页面状态应彻底移除")
    void testVictimRemovesCompletely() {
        replacer.Pin(1);
        replacer.Unpin(1);
        replacer.Victim(); // 驱逐1

        assertThatThrownBy(() -> replacer.Unpin(1))
                .isInstanceOf(RuntimeException.class);
        assertThat(replacer.size()).isEqualTo(0);
    }

    @Test
    @DisplayName("交替固定/取消固定后的容量计算")
    void testAlternatingPinUnpinSize() {
        replacer.Pin(1);
        replacer.Unpin(1); // size=1 (LRU)
        replacer.Pin(1);   // size=1 (Pinned)
        replacer.Pin(2);   // size=2 (Pinned)
        replacer.Unpin(2); // size=2 (1 Pinned, 1 LRU)

        assertThat(replacer.size()).isEqualTo(2);
    }

    @Test
    @DisplayName("批量操作后的内部一致性")
    void testBulkOperationsConsistency() {
        IntStream.range(0, 5).forEach(i -> {
            replacer.Pin(i);
            replacer.Unpin(i);
        });

        assertThat(replacer.size()).isEqualTo(5);
        assertThat(replacer.Victim()).isEqualTo(0);

        replacer.Pin(1);
        replacer.Unpin(1);

        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(3);
        assertThat(replacer.Victim()).isEqualTo(4);
        assertThat(replacer.Victim()).isEqualTo(1); // 重新加入后变为最后
    }
}