package edu.sustech.cs307.tuple;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import edu.sustech.cs307.meta.TabCol;
import edu.sustech.cs307.value.Value;

import java.util.ArrayList;
import java.util.List;

public class TempTuple extends Tuple {
    private List<Value> values;

    public TempTuple(List<Value> values) {
        this.values = values;
    }

    @Override
    public Value getValue(TabCol tabCol) throws DBException {
        throw new DBException(ExceptionTypes.GetValueFromTempTuple());
    }

    @Override
    public TabCol[] getTupleSchema() {
        return null;
    }

    @Override
    public Value[] getValues() throws DBException {
        return this.values.toArray(new Value[0]);
    }
}