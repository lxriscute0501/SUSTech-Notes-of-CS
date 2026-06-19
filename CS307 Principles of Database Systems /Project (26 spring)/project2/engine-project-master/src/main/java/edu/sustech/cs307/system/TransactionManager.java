package edu.sustech.cs307.system;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;


public class TransactionManager {

    private final DBManager dbManager;
    private Path transactionSnapshot;


    public TransactionManager(DBManager dbManager) {
        this.dbManager = dbManager;
    }


    public void begin() throws DBException {
        transactionSnapshot = createSnapshot();
        //TODO: Complete the method
    }


    public void commit() throws DBException {
        dbManager.persistRuntimeState();
        //TODO: Complete the method.
    }


    public void rollback() throws DBException {
       //TODO: finish roll back
        throw new UnsupportedOperationException("TODO: implement rollback in TransactionManager_framework");
    }


    public void savepoint(String savepointName) throws DBException {
        //TODO: finish save point
        throw new UnsupportedOperationException("TODO: implement savepoint in TransactionManager_framework");

    }


    public void rollbackToSavepoint(String savepointName) throws DBException {
        //TODO: rollbackToSavepoint
        throw new UnsupportedOperationException("TODO: implement rollbackToSavepoint in TransactionManager_framework");
    }


    public void releaseSavepoint(String savepointName) throws DBException {
        //TODO: releaseSavepoint
        throw new UnsupportedOperationException("TODO: implement releaseSavepoint in TransactionManager_framework");
    }

    private Path createSnapshot() throws DBException {
        dbManager.persistRuntimeState();
        Path snapshotDir;
        try {
            snapshotDir = Files.createTempDirectory("cs307-txn-");
            copyDirectoryContents(getDbRoot(), snapshotDir);
        } catch (IOException e) {
            throw new DBException(ExceptionTypes.BadIOError(e.getMessage()));
        }
        return snapshotDir;
    }

    private Path getDbRoot() {
        return Path.of(dbManager.getDiskManager().getCurrentDir());
    }

    private void copyDirectoryContents(Path sourceRoot, Path targetRoot) throws IOException {
        if (!Files.exists(sourceRoot)) {
            Files.createDirectories(targetRoot);
            return;
        }
        Files.createDirectories(targetRoot);
        try (var paths = Files.walk(sourceRoot)) {
            for (Path source : paths.toList()) {
                Path relative = sourceRoot.relativize(source);
                Path target = targetRoot.resolve(relative);
                if (Files.isDirectory(source)) {
                    Files.createDirectories(target);
                } else {
                    Files.createDirectories(target.getParent());
                    Files.copy(source, target, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
                }
            }
        }
    }
}
