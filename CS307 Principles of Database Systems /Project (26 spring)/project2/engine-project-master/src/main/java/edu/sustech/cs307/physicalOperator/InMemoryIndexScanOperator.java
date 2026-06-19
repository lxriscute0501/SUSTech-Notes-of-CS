package edu.sustech.cs307.physicalOperator;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.index.InMemoryOrderedIndex;
import edu.sustech.cs307.record.Record;
import edu.sustech.cs307.tuple.Tuple;
import edu.sustech.cs307.meta.ColumnMeta;

import java.util.ArrayList;

public class InMemoryIndexScanOperator implements PhysicalOperator {

    private InMemoryOrderedIndex index;

    public InMemoryIndexScanOperator(InMemoryOrderedIndex index) {
        this.index = index;
    }

    @Override
    public boolean hasNext() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'hasNext'");
    }

    @Override
    public void Begin() throws DBException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'Begin'");
    }

    @Override
    public void Next() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'Next'");
    }

    @Override
    public Tuple Current() { // Return Tuple
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'Current'");
    }

    @Override
    public void Close() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'Close'");
    }

    @Override
    public ArrayList<ColumnMeta> outputSchema() {
        // TODO Auto-generated method stub
        return null;
    }
}
