package value;


import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import edu.sustech.cs307.value.Value;
import edu.sustech.cs307.value.ValueComparer;
import edu.sustech.cs307.value.ValueType;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.nio.ByteBuffer;
import java.util.stream.Stream;

import static org.assertj.core.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ValueComparerTest {

    @Test
    @DisplayName("整数比较1")
    void testCompareIntegerEqual() throws DBException {
        Value v1 = new Value(10L);
        Value v2 = new Value(10L);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(0);  // 10 == 10
    }

    @Test
    @DisplayName("整数比较2")
    void testCompareIntegerGreaterThan() throws DBException {
        Value v1 = new Value(15L);
        Value v2 = new Value(10L);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(1);  // 15 > 10
    }

    @Test
    @DisplayName("整数比较3")
    void testCompareIntegerLessThan() throws DBException {
        Value v1 = new Value(5L);
        Value v2 = new Value(10L);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(-1);  // 5 < 10
    }

    @Test
    @DisplayName("浮点数比较1")
    void testCompareFloatEqual() throws DBException {
        Value v1 = new Value(10.5);
        Value v2 = new Value(10.5);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(0);  // 10.5 == 10.5
    }

    @Test
    @DisplayName("浮点数比较2")
    void testCompareFloatGreaterThan() throws DBException {
        Value v1 = new Value(15.5);
        Value v2 = new Value(10.5);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(1);  // 15.5 > 10.5
    }

    @Test
    @DisplayName("浮点数比较3")
    void testCompareFloatLessThan() throws DBException {
        Value v1 = new Value(5.5);
        Value v2 = new Value(10.5);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(-1);  // 5.5 < 10.5
    }

    @Test
    @DisplayName("字符串比较1")
    void testCompareStringEqual() throws DBException {
        Value v1 = new Value("apple");
        Value v2 = new Value("apple");

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(0);  // "apple" == "apple"
    }

    @Test
    @DisplayName("字符串比较2")
    void testCompareStringGreaterThan() throws DBException {
        Value v1 = new Value("banana");
        Value v2 = new Value("apple");

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(1);  // "banana" > "apple"
    }

    @Test
    @DisplayName("字符串比较3")
    void testCompareStringLessThan() throws DBException {
        Value v1 = new Value("apple");
        Value v2 = new Value("banana");

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(-1);  // "apple" < "banana"
    }

    @Test
    @DisplayName("V1为Null")
    void testCompareNullV1() throws DBException {
        Value v1 = null;
        Value v2 = new Value(10L);

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(-1);  // v1 is null, so return -1
    }

    @Test
    @DisplayName("V2为Null")
    void testCompareNullV2() throws DBException {
        Value v1 = new Value(10L);
        Value v2 = null;

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(1);  // v2 is null, so return 1
    }

    @Test
    @DisplayName("两者同时为null")
    void testCompareNullBoth() throws DBException {
        Value v1 = null;
        Value v2 = null;

        int result = ValueComparer.compare(v1, v2);

        assertThat(result).isEqualTo(-1);  // Both null, return -1
    }

    @Test
    @DisplayName("不同类型比较")
    void testCompareDifferentTypes() {
        Value v1 = new Value(10L);
        Value v2 = new Value(10.0);

        assertThatThrownBy(() -> ValueComparer.compare(v1, v2))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("WRONG_COMPARISON_TYPE");
    }

    @Test
    @DisplayName("测试使用Object和ValueType构造函数")
    public void testConstructorWithObjectAndType() {
        Value value = new Value(123L, ValueType.INTEGER);
        assertThat(value.value).isEqualTo(123L);
        assertThat(value.type).isEqualTo(ValueType.INTEGER);

        value = new Value(123.45, ValueType.FLOAT);
        assertThat(value.value).isEqualTo(123.45);
        assertThat(value.type).isEqualTo(ValueType.FLOAT);

        value = new Value("test", ValueType.CHAR);
        assertThat(value.value).isEqualTo("test");
        assertThat(value.type).isEqualTo(ValueType.CHAR);
    }

    @Test
    @DisplayName("测试使用Long构造函数")
    public void testConstructorWithLong() {
        Value value = new Value(123L);
        assertThat(value.value).isEqualTo(123L);
        assertThat(value.type).isEqualTo(ValueType.INTEGER);
    }

    @Test
    @DisplayName("测试使用Double构造函数")
    public void testConstructorWithDouble() {
        Value value = new Value(123.45);
        assertThat(value.value).isEqualTo(123.45);
        assertThat(value.type).isEqualTo(ValueType.FLOAT);
    }

    @Test
    @DisplayName("测试使用String构造函数")
    public void testConstructorWithString() {
        Value value = new Value("test");
        assertThat(value.value).isEqualTo("test");
        assertThat(value.type).isEqualTo(ValueType.CHAR);
    }

    @ParameterizedTest
    @MethodSource("provideToByteData")
    @DisplayName("测试ToByte方法")
    public void testToByte(Object value, ValueType type, byte[] expectedBytes) {
        Value valueObj = new Value(value, type);
        assertThat(valueObj.ToByte()).isEqualTo(expectedBytes);
    }

    private static Stream<Arguments> provideToByteData() {
        return Stream.of(
                Arguments.of(123L, ValueType.INTEGER, ByteBuffer.allocate(8).putLong(123L).array()),
                Arguments.of(123.45, ValueType.FLOAT, ByteBuffer.allocate(8).putDouble(123.45).array()),
                Arguments.of("test", ValueType.CHAR, "test".getBytes())
        );
    }

    @ParameterizedTest
    @MethodSource("provideFromByteData")
    @DisplayName("测试FromByte方法")
    public void testFromByte(byte[] bytes, ValueType type, Object expectedValue) {
        Value valueObj = Value.FromByte(bytes, type);
        assertThat(valueObj.value).isEqualTo(expectedValue);
        assertThat(valueObj.type).isEqualTo(type);
    }

    private static Stream<Arguments> provideFromByteData() {
        return Stream.of(
                Arguments.of(ByteBuffer.allocate(8).putLong(123L).array(), ValueType.INTEGER, 123L),
                Arguments.of(ByteBuffer.allocate(8).putDouble(123.45).array(), ValueType.FLOAT, 123.45),
                Arguments.of("test".getBytes(), ValueType.CHAR, "test")
        );
    }

    @Test
    @DisplayName("测试ToByte方法在不支持的类型时抛出异常")
    public void testToByteWithUnsupportedType() {
        Value value = new Value(123L, ValueType.UNKNOWN);
        assertThrows(RuntimeException.class, value::ToByte);
    }

    @Test
    @DisplayName("测试FromByte方法在不支持的类型时抛出异常")
    public void testFromByteWithUnsupportedType() {
        byte[] bytes = ByteBuffer.allocate(8).putLong(123L).array();
        assertThrows(RuntimeException.class, () -> Value.FromByte(bytes, ValueType.UNKNOWN));
    }
}