package edu.sustech.cs307.exception;

public class DBException extends Exception {
    public DBException(ExceptionTypes exceptionType) {
        super(String.format(
                "%s: %s", exceptionType.name(), exceptionType.GetErrorResult()
        ));
    }
}
