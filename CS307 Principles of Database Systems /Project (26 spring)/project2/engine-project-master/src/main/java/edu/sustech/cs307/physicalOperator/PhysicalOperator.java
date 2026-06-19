package edu.sustech.cs307.physicalOperator;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.tuple.Tuple;

import java.util.ArrayList;

public interface PhysicalOperator {
    boolean hasNext() throws DBException;

    void Begin() throws DBException;

    void Next() throws DBException;

    Tuple Current();

    void Close();

    ArrayList<ColumnMeta> outputSchema();
}
