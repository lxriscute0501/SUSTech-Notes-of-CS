package edu.sustech.cs307.physicalOperator;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.record.Record;
import edu.sustech.cs307.tuple.Tuple;

import java.util.ArrayList;

public class IndexScanOperator implements PhysicalOperator {
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
