package edu.sustech.cs307.logicalOperator.ddl;

import edu.sustech.cs307.exception.DBException;

public interface DMLExecutor {
    void execute() throws DBException;
}
