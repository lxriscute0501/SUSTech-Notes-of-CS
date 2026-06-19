package edu.sustech.cs307.logicalOperator;

import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.expression.operators.relational.ExpressionList;
import net.sf.jsqlparser.schema.Column;

import java.util.Collections;

public class LogicalInsertOperator extends LogicalOperator {
    public final String tableName;
    public final ExpressionList<Column> columns;
    public final Expression values;

    public LogicalInsertOperator(String tableName, ExpressionList<Column> columns, Expression values) {
        super(Collections.emptyList());
        this.tableName = tableName;
        this.columns = columns;
        this.values = values;
    }

    @Override
    public String toString() {
        return "InsertOperator(table=" + tableName + ", columns=" + columns + ", values=" + values + ")";
    }
}
