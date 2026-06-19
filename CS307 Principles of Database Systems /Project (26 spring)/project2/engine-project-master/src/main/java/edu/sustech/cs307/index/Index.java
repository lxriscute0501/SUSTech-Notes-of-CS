package edu.sustech.cs307.index;

import java.util.Iterator;
import java.util.Map.Entry;

import edu.sustech.cs307.record.RID;
import edu.sustech.cs307.value.Value;

public interface Index {
    RID EqualTo(Value value);

    Iterator<Entry<Value, RID>> LessThan(Value value, boolean isEqual);

    Iterator<Entry<Value, RID>> MoreThan(Value value, boolean isEqual);

    Iterator<Entry<Value, RID>> Range(Value low, Value high, boolean leftEqual, boolean rightEqual);
}
