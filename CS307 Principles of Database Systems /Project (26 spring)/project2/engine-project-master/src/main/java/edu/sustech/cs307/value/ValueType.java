package edu.sustech.cs307.value;

public enum ValueType {
    CHAR,
    INTEGER,
    FLOAT,
    UNKNOWN;

    /**
     * 返回当前值类型的字符串表示。
     * 
     * @return 当前值类型的名称，可能为 "char"、"int"、"float" 或 "unknown"。
     */
    @Override
    public String toString() {
        return switch (this) {
            case CHAR -> "char";
            case INTEGER -> "int";
            case FLOAT -> "float";
            case UNKNOWN -> "unknown";
        };
    }
}
