package edu.sustech.cs307.exception;
import net.sf.jsqlparser.expression.Expression;
import edu.sustech.cs307.value.ValueType;

public enum ExceptionTypes {
    WRONG_COMPARISON_TYPE,
    BAD_IO_TYPE,
    INVALID_SQL, UNSUPPORTED_COMMAND_TYPE,
    UNSUPPORTED_OPERATOR_TYPE,
    UNSUPPORTED_VALUE_TYPE,
    UNSUPPORTED_EXPRESSION,
    UNABLE_LOAD_METADATA,
    UNABLE_SAVE_METADATA,
    TABLE_ALREADY_EXIST,
    TABLE_DOES_NOT_EXIST,
    COLUMN_ALREADY_EXIST,
    COLUMN_DOES_NOT_EXIST,
    TABLE_HAS_NO_COLUMN,
    INVALID_TABLE_WIDTH,
    INSERT_COLUMN_SIZE_NOT_MATCH,
    INSERT_COLUMN_NAME_NOT_MATCH,
    INSERT_COLUMN_TYPE_NOT_MATCH,
    GET_VALUE_FROM_TEMP_TUPLE,
    NOT_SUPPORTED_OPERATION
    ;

    private String error_result;

    public void SetErrorResult(String error_result) {
        this.error_result = error_result;
    }

    public String GetErrorResult() {
        return error_result;
    }

    static public ExceptionTypes WrongComparisonError(ValueType left, ValueType right) {
        WRONG_COMPARISON_TYPE.SetErrorResult(
                String.format("The type of value not match, left is %s, right is %s", left, right));
        return WRONG_COMPARISON_TYPE;
    }

    static public ExceptionTypes BadIOError(String IOExceptionMessage) {
        BAD_IO_TYPE.SetErrorResult(
                String.format(
                        "I/O is error, please check the message from system.\n Message:%s", IOExceptionMessage));
        return BAD_IO_TYPE;
    }

    static public ExceptionTypes InvalidSQL(String sql, String reason) {
        INVALID_SQL.SetErrorResult(
                String.format("Invalid SQL: %s, the reason is: %s", sql, reason));
        return INVALID_SQL;
    }

    static public ExceptionTypes UnsupportedCommand(String type) {
        UNSUPPORTED_COMMAND_TYPE.SetErrorResult(
                String.format("Unsupported command type: %s", type));
        return UNSUPPORTED_COMMAND_TYPE;
    }

    static public ExceptionTypes UnableLoadMetadata(String reason) {
        UNABLE_LOAD_METADATA.SetErrorResult(
                String.format("Unable to load metadata, the reason is: %s", reason));
        return UNABLE_LOAD_METADATA;
    }

    static public ExceptionTypes UnableSaveMetadata(String reason) {
        UNABLE_SAVE_METADATA.SetErrorResult(
                String.format("Unable to save metadata, the reason is: %s", reason));
        return UNABLE_SAVE_METADATA;
    }

    static public ExceptionTypes TableAlreadyExist(String tableName) {
        TABLE_ALREADY_EXIST.SetErrorResult(
                String.format("Table is already exist: %s", tableName));
        return TABLE_ALREADY_EXIST;
    }

    static public ExceptionTypes TableDoesNotExist(String tableName) {
        TABLE_DOES_NOT_EXIST.SetErrorResult(
                String.format("Table does not exist: %s", tableName));
        return TABLE_DOES_NOT_EXIST;
    }

    static public ExceptionTypes ColumnAlreadyExist(String columnName) {
        COLUMN_ALREADY_EXIST.SetErrorResult(
                String.format("Column is already exist: %s", columnName));
        return COLUMN_ALREADY_EXIST;
    }

    static public ExceptionTypes ColumnDoesNotExist(String columnName) {
        COLUMN_DOES_NOT_EXIST.SetErrorResult(
                String.format("Column does not exist: %s", columnName));
        return COLUMN_DOES_NOT_EXIST;
    }

    static public ExceptionTypes TableHasNoColumn(String tableName) {
        TABLE_HAS_NO_COLUMN.SetErrorResult(
                String.format("Table has no column: %s", tableName));
        return TABLE_HAS_NO_COLUMN;
    }

    static public ExceptionTypes InvalidTableWidth(int width) {
        INVALID_TABLE_WIDTH.SetErrorResult(
                String.format("Invalid table width: %d", width));
        return INVALID_TABLE_WIDTH;
    }

    static public ExceptionTypes UnsupportedOperator(String operatorType) {
        UNSUPPORTED_OPERATOR_TYPE.SetErrorResult(
                String.format("Unsupported operator type: %s", operatorType));
        return UNSUPPORTED_OPERATOR_TYPE;
    }

    static public ExceptionTypes InsertColumnSizeMismatch() {
        INSERT_COLUMN_SIZE_NOT_MATCH.SetErrorResult(
                String.format("Insert column size not match"));
        return INSERT_COLUMN_SIZE_NOT_MATCH;
    }

    static public ExceptionTypes InsertColumnNameMismatch() {
        INSERT_COLUMN_NAME_NOT_MATCH.SetErrorResult(
                String.format("Insert column name not match"));
        return INSERT_COLUMN_NAME_NOT_MATCH;
    }

    static public ExceptionTypes InsertColumnTypeMismatch() {
        INSERT_COLUMN_TYPE_NOT_MATCH.SetErrorResult(
                String.format("Insert column type not match"));
        return INSERT_COLUMN_TYPE_NOT_MATCH;
    }

    static public ExceptionTypes UnsupportedValueType(ValueType valueType) {
        UNSUPPORTED_VALUE_TYPE.SetErrorResult(
                String.format("Unsupported value type: %s", valueType));
        return UNSUPPORTED_VALUE_TYPE;
    }

    static public ExceptionTypes GetValueFromTempTuple() {
        GET_VALUE_FROM_TEMP_TUPLE.SetErrorResult(
                String.format("Get value from temp tuple"));
        return GET_VALUE_FROM_TEMP_TUPLE;
    }

    static public ExceptionTypes UnsupportedExpression(Expression expression) {
        UNSUPPORTED_EXPRESSION.SetErrorResult(
                String.format("Unsupported expression: %s", expression));
        return UNSUPPORTED_EXPRESSION;
    }

    static public ExceptionTypes NotSupportedOperation(Expression expression) {
        NOT_SUPPORTED_OPERATION.SetErrorResult(
                String.format("Unsupported operation: %s", expression)
        );
        return NOT_SUPPORTED_OPERATION;
    }
}
