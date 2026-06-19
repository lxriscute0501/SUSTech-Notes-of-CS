package edu.sustech.cs307.physicalOperator;

import java.util.ArrayList;
import java.util.Collection;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.tuple.Tuple;
import net.sf.jsqlparser.expression.Expression;

public class NestedLoopJoinOperator implements PhysicalOperator {

    private PhysicalOperator leftOperator;
    private PhysicalOperator rightOperator;
    private Collection<Expression> expr;

    public NestedLoopJoinOperator(PhysicalOperator leftOperator, PhysicalOperator rightOperator,
            Collection<Expression> expr) {
        this.leftOperator = leftOperator;
        this.rightOperator = rightOperator;
        this.expr = expr;
    }

    @Override
    public boolean hasNext() {
        return false;
    }

    @Override
    public void Begin() throws DBException {

    }

    @Override
    public void Next() {

    }

    @Override
    public Tuple Current() {
        return null;
    }

    @Override
    public void Close() {

    }

    @Override
    public ArrayList<ColumnMeta> outputSchema() {
        return null;
    }
}
