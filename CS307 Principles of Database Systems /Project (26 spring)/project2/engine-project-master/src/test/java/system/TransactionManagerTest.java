package system;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.logicalOperator.LogicalOperator;
import edu.sustech.cs307.meta.MetaManager;
import edu.sustech.cs307.optimizer.LogicalPlanner;
import edu.sustech.cs307.optimizer.PhysicalPlanner;
import edu.sustech.cs307.physicalOperator.PhysicalOperator;
import edu.sustech.cs307.storage.BufferPool;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.storage.replacer.ClockReplacer;
import edu.sustech.cs307.storage.replacer.PageReplacer;
import edu.sustech.cs307.system.*;
import edu.sustech.cs307.tuple.Tuple;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.function.IntFunction;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatCode;
import static org.assertj.core.api.Assertions.assertThatThrownBy;

class TransactionManagerTest {

    @TempDir
    Path tempDir;

    @Test
    @DisplayName("BEGIN + SAVEPOINT + ROLLBACK TO + COMMIT 应保留回滚点之前的数据")
    void testRollbackToSavepointThenCommit() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "CREATE TABLE users (id int)");
        executeStatement(dbManager, "BEGIN");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (1)");
        executeStatement(dbManager, "SAVEPOINT alice_added");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (2)");
        executeStatement(dbManager, "SAVEPOINT bob_added");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (3)");
        executeStatement(dbManager, "ROLLBACK TO SAVEPOINT bob_added");
        executeStatement(dbManager, "COMMIT");

        assertThat(selectIds(dbManager, "SELECT * FROM users"))
                .containsExactly(1L, 2L);
    }

    @Test
    @DisplayName("ROLLBACK 应恢复到 BEGIN 之前的状态")
    void testRollbackRestoresStateBeforeBegin() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "CREATE TABLE users (id int)");
        executeStatement(dbManager, "BEGIN");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (10)");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (20)");
        executeStatement(dbManager, "ROLLBACK");

        assertThat(selectIds(dbManager, "SELECT * FROM users")).isEmpty();
    }

    @Test
    @DisplayName("RELEASE SAVEPOINT 后不能再回滚到该保存点")
    void testReleaseSavepointRemovesRollbackTarget() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "CREATE TABLE users (id int)");
        executeStatement(dbManager, "BEGIN");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (1)");
        executeStatement(dbManager, "SAVEPOINT keep_rows");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (2)");
        executeStatement(dbManager, "RELEASE SAVEPOINT keep_rows");

        assertThatThrownBy(() -> executeStatement(dbManager, "ROLLBACK TO SAVEPOINT keep_rows"))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("SAVEPOINT_DOES_NOT_EXIST");

        executeStatement(dbManager, "COMMIT");

        assertThat(selectIds(dbManager, "SELECT * FROM users"))
                .containsExactly(1L, 2L);
    }

    @Test
    @DisplayName("SAVEPOINT 必须在事务中使用")
    void testSavepointRequiresTransaction() throws DBException {
        DBManager dbManager = buildDbManager();
        executeStatement(dbManager, "CREATE TABLE users (id int)");

        assertThatThrownBy(() -> executeStatement(dbManager, "SAVEPOINT outside_tx"))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("TRANSACTION_REQUIRED");
    }

    @Test
    @DisplayName("事务中再次 BEGIN 应报错")
    void testBeginInsideTransactionShouldFail() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "BEGIN");
        assertThatThrownBy(() -> executeStatement(dbManager, "BEGIN"))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("TRANSACTION_ALREADY_ACTIVE");
    }

    @Test
    @DisplayName("ROLLBACK TO SAVEPOINT 必须在事务中使用")
    void testRollbackToSavepointRequiresTransaction() throws DBException {
        DBManager dbManager = buildDbManager();
        executeStatement(dbManager, "CREATE TABLE users (id int)");

        assertThatThrownBy(() -> executeStatement(dbManager, "ROLLBACK TO SAVEPOINT no_tx"))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("TRANSACTION_REQUIRED");
    }

    @Test
    @DisplayName("RELEASE SAVEPOINT 必须在事务中使用")
    void testReleaseSavepointRequiresTransaction() throws DBException {
        DBManager dbManager = buildDbManager();
        executeStatement(dbManager, "CREATE TABLE users (id int)");

        assertThatThrownBy(() -> executeStatement(dbManager, "RELEASE SAVEPOINT no_tx"))
                .isInstanceOf(DBException.class)
                .hasMessageContaining("TRANSACTION_REQUIRED");
    }

    @Test
    @DisplayName("ROLLBACK TO SAVEPOINT 后目标保存点仍然可继续使用")
    void testRollbackToSavepointKeepsTargetActive() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "CREATE TABLE users (id int)");
        executeStatement(dbManager, "BEGIN");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (1)");
        executeStatement(dbManager, "SAVEPOINT keep_point");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (2)");
        executeStatement(dbManager, "ROLLBACK TO SAVEPOINT keep_point");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (3)");
        executeStatement(dbManager, "ROLLBACK TO SAVEPOINT keep_point");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (4)");
        executeStatement(dbManager, "COMMIT");

        assertThat(selectIds(dbManager, "SELECT * FROM users"))
                .containsExactly(1L, 4L);
    }

    @Test
    @DisplayName("同名 SAVEPOINT 应按栈语义遮蔽旧保存点")
    void testDuplicateSavepointNamesFollowStackSemantics() throws DBException {
        DBManager dbManager = buildDbManager();

        executeStatement(dbManager, "CREATE TABLE users (id int)");
        executeStatement(dbManager, "BEGIN");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (1)");
        executeStatement(dbManager, "SAVEPOINT same_name");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (2)");
        executeStatement(dbManager, "SAVEPOINT same_name");
        executeStatement(dbManager, "INSERT INTO users (id) VALUES (3)");

        executeStatement(dbManager, "ROLLBACK TO SAVEPOINT same_name");
        assertThat(selectIds(dbManager, "SELECT * FROM users"))
                .containsExactly(1L, 2L);

        executeStatement(dbManager, "RELEASE SAVEPOINT same_name");
        executeStatement(dbManager, "ROLLBACK TO SAVEPOINT same_name");
        executeStatement(dbManager, "COMMIT");

        assertThat(selectIds(dbManager, "SELECT * FROM users"))
                .containsExactly(1L);
    }

    @Test
    @DisplayName("事务外 COMMIT 和 ROLLBACK 不应报错")
    void testCommitAndRollbackOutsideTransactionAreNoOps() throws DBException {
        DBManager dbManager = buildDbManager();
        executeStatement(dbManager, "CREATE TABLE users (id int)");

        assertThatCode(() -> executeStatement(dbManager, "COMMIT")).doesNotThrowAnyException();
        assertThatCode(() -> executeStatement(dbManager, "ROLLBACK")).doesNotThrowAnyException();
        assertThat(selectIds(dbManager, "SELECT * FROM users")).isEmpty();
    }

    private DBManager buildDbManager() throws DBException {
        HashMap<String, Integer> fileOffsets = new HashMap<>();
        DiskManager diskManager = new DiskManager(tempDir.toString(), fileOffsets);
        IntFunction<PageReplacer> replacerFactory = ClockReplacer::new;
        BufferPool bufferPool = new BufferPool(16, diskManager, replacerFactory.apply(16));
        RecordManager recordManager = new RecordManager(diskManager, bufferPool);
        MetaManager metaManager = new MetaManager(tempDir.resolve("meta").toString());
        DBManager dbManager = new DBManager(diskManager, bufferPool, recordManager, metaManager, null,
                replacerFactory);
        dbManager.setTransactionManager(new TransactionManager(dbManager));
        return dbManager;
    }

    private void executeStatement(DBManager dbManager, String sql) throws DBException {
        LogicalOperator logicalOperator = LogicalPlanner.resolveAndPlan(dbManager, sql);
        if (logicalOperator == null) {
            return;
        }
        PhysicalOperator physicalOperator = PhysicalPlanner.generateOperator(dbManager, logicalOperator);
        physicalOperator.Begin();
        while (physicalOperator.hasNext()) {
            physicalOperator.Next();
            physicalOperator.Current();
        }
        physicalOperator.Close();
        dbManager.getBufferPool().FlushAllPages("");
    }

    private List<Long> selectIds(DBManager dbManager, String sql) throws DBException {
        LogicalOperator logicalOperator = LogicalPlanner.resolveAndPlan(dbManager, sql);
        PhysicalOperator physicalOperator = PhysicalPlanner.generateOperator(dbManager, logicalOperator);
        List<Long> ids = new ArrayList<>();
        physicalOperator.Begin();
        while (physicalOperator.hasNext()) {
            physicalOperator.Next();
            Tuple tuple = physicalOperator.Current();
            ids.add((Long) tuple.getValues()[0].value);
        }
        physicalOperator.Close();
        return ids;
    }
}
