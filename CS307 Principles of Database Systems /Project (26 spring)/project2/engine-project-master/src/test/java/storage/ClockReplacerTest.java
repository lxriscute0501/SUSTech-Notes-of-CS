package storage;

import edu.sustech.cs307.storage.replacer.ClockReplacer;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.stream.IntStream;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;

class ClockReplacerTest {

    private ClockReplacer replacer;

    @BeforeEach
    void setUp() {
        replacer = new ClockReplacer(5);
    }

    private void pinAll(int... frameIds) {
        for (int frameId : frameIds) {
            replacer.Pin(frameId);
        }
    }

    private void unpinAll(int... frameIds) {
        for (int frameId : frameIds) {
            replacer.Unpin(frameId);
        }
    }

    private void pinAndUnpinAll(int... frameIds) {
        pinAll(frameIds);
        unpinAll(frameIds);
    }

    @Test
    @DisplayName("空 CLOCK 不返回牺牲页")
    void testVictimWhenEmpty() {
        assertThat(replacer.Victim()).isEqualTo(-1);
    }

    @Test
    @DisplayName("固定页面直到容量上限")
    void testPinUntilCapacity() {
        IntStream.rangeClosed(1, 5).forEach(replacer::Pin);

        assertThat(replacer.size()).isEqualTo(5);
        assertThatThrownBy(() -> replacer.Pin(6))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("REPLACER IS FULL");
    }

    @Test
    @DisplayName("重复固定同一页面不会重复计数")
    void testDuplicatePinKeepsSize() {
        replacer.Pin(1);
        replacer.Pin(1);

        assertThat(replacer.size()).isEqualTo(1);
        replacer.Unpin(1);
        assertThat(replacer.Victim()).isEqualTo(1);
    }

    @Test
    @DisplayName("取消固定未知页面会失败")
    void testUnpinUnknownFrame() {
        assertThatThrownBy(() -> replacer.Unpin(1))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("UNPIN PAGE NOT FOUND");
    }

    @Test
    @DisplayName("CLOCK 会先给所有可驱逐页面一次二次机会")
    void testVictimUsesSecondChance() {
        pinAndUnpinAll(1, 2, 3);

        assertThat(replacer.Victim()).isEqualTo(1);
        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.size()).isEqualTo(1);
    }

    @Test
    @DisplayName("首次驱逐前的重新访问不会退化成LRU")
    void testReaccessBeforeFirstVictimDoesNotBehaveLikeLru() {
        pinAndUnpinAll(1, 2, 3);

        replacer.Pin(1);
        replacer.Unpin(1);

        assertThat(replacer.Victim()).isEqualTo(1);
        assertThat(replacer.Victim()).isEqualTo(2);
    }

    @Test
    @DisplayName("重新访问页面会刷新引用位")
    void testRepinRefreshesReferenceBit() {
        pinAndUnpinAll(1, 2, 3);

        assertThat(replacer.Victim()).isEqualTo(1);

        replacer.Pin(2);
        replacer.Unpin(2);

        assertThat(replacer.Victim()).isEqualTo(3);
        assertThat(replacer.Victim()).isEqualTo(2);
    }

    @Test
    @DisplayName("从可驱逐状态重新固定后不能再被驱逐")
    void testRepinExistingEvictableFrameMakesItPinned() {
        replacer = new ClockReplacer(2);
        pinAndUnpinAll(1, 2);

        replacer.Pin(1);

        assertThat(replacer.size()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(-1);

        replacer.Unpin(1);
        assertThat(replacer.Victim()).isEqualTo(1);
    }

    @Test
    @DisplayName("所有页面固定时无法驱逐")
    void testAllPinnedNoVictim() {
        IntStream.range(0, 5).forEach(replacer::Pin);

        assertThat(replacer.Victim()).isEqualTo(-1);

        replacer.Unpin(3);
        assertThat(replacer.Victim()).isEqualTo(3);
    }

    @Test
    @DisplayName("牺牲页移除后必须重新固定才能再次加入")
    void testVictimRemovesFrameCompletely() {
        replacer.Pin(1);
        replacer.Unpin(1);

        assertThat(replacer.Victim()).isEqualTo(1);
        assertThat(replacer.size()).isEqualTo(0);
        assertThatThrownBy(() -> replacer.Unpin(1))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("UNPIN PAGE NOT FOUND");
    }

    @Test
    @DisplayName("重复取消固定同一页面会失败")
    void testDoubleUnpinFails() {
        replacer.Pin(1);
        replacer.Unpin(1);

        assertThatThrownBy(() -> replacer.Unpin(1))
                .isInstanceOf(RuntimeException.class)
                .hasMessageContaining("UNPIN PAGE NOT FOUND");
    }

    @Test
    @DisplayName("固定页面在扫描中会被跳过")
    void testPinnedFramesAreSkipped() {
        replacer.Pin(1);
        replacer.Pin(2);
        replacer.Pin(3);

        replacer.Unpin(1);
        replacer.Unpin(2);

        assertThat(replacer.Victim()).isEqualTo(1);
        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(-1);
    }

    @Test
    @DisplayName("驱逐后可以加入新的页面")
    void testInsertAfterEviction() {
        replacer = new ClockReplacer(3);
        IntStream.range(0, 3).forEach(replacer::Pin);
        IntStream.range(0, 3).forEach(replacer::Unpin);

        assertThat(replacer.Victim()).isEqualTo(0);

        replacer.Pin(3);
        assertThat(replacer.size()).isEqualTo(3);
    }

    @Test
    @DisplayName("驱逐后插入新页面不应破坏时钟指针顺序")
    void testInsertionAfterEvictionKeepsClockOrder() {
        replacer = new ClockReplacer(3);
        pinAndUnpinAll(1, 2, 3);

        assertThat(replacer.Victim()).isEqualTo(1);

        replacer.Pin(4);
        replacer.Unpin(4);

        assertThat(replacer.Victim()).isEqualTo(2);
        assertThat(replacer.Victim()).isEqualTo(3);
        assertThat(replacer.Victim()).isEqualTo(4);
    }

    @Test
    @DisplayName("固定与取消固定切换不应改变驻留页计数")
    void testSizeRemainsStableAcrossStateTransitions() {
        replacer.Pin(1);
        assertThat(replacer.size()).isEqualTo(1);

        replacer.Unpin(1);
        assertThat(replacer.size()).isEqualTo(1);

        replacer.Pin(1);
        assertThat(replacer.size()).isEqualTo(1);
        assertThat(replacer.Victim()).isEqualTo(-1);
    }
}
