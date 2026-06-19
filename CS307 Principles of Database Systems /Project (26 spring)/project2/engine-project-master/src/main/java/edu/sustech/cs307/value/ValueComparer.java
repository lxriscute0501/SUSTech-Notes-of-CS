package edu.sustech.cs307.value;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;

/*
*
* ValueComparer::compare
*
* When v1 > v2, return 1
* When v1 = v2, return 0
* When v1 < v2, return -1
* ATTENTION: First: If v1 is null, always return -1; Second: If v2 is null, always return 1.
* */

public class ValueComparer {
    public static int compare(Value v1, Value v2) throws DBException {
        if (v1 == null) {
            return -1;
        }
        if (v2 == null) {
            return 1;
        }
        if (v1.type != v2.type) {
            throw new DBException(ExceptionTypes.WrongComparisonError(v1.type, v2.type));
        }
        switch (v1.type) {
            case INTEGER:
                Long i1 = (Long) v1.value;
                Long i2 = (Long) v2.value;
                return Long.compare(i1, i2);
            case FLOAT:
                Double d1 = (Double) v1.value;
                Double d2 = (Double) v2.value;
                return Double.compare(d1, d2);
            case CHAR:
                String s1 = (String) v1.value;
                s1=s1.trim();
                String s2 = (String) v2.value;
                s2=s2.trim();
                return s1.compareTo(s2);
            default:
                throw new DBException(ExceptionTypes.WrongComparisonError(v1.type, v2.type));
        }
    }
}
