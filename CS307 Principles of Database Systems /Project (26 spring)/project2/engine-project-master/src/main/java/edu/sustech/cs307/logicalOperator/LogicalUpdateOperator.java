package edu.sustech.cs307.logicalOperator;

import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.statement.update.UpdateSet;

import java.util.Collections;
import java.util.List;

public class LogicalUpdateOperator extends LogicalOperator {
    private final String tableName;
    private final List<UpdateSet> columns;
    private final Expression expressions;

    public LogicalUpdateOperator(LogicalOperator child, String tableName, List<UpdateSet> columns,
                                 Expression expressions) {
        super(Collections.singletonList(child));
        this.tableName = tableName;
        this.columns = columns;
        this.expressions = expressions;
    }

    public String getTableName() {
        return tableName;
    }

    public List<UpdateSet> getColumns() {
        return columns;
    }

    public Expression getExpression() {
        return expressions;
    }

    @Override
    public String toString() {
        return "UpdateOperator(table=" + tableName + ", columns=" + columns + ", expressions=" + expressions
                + ")\n ├── " + childern.get(0);
    }
}
