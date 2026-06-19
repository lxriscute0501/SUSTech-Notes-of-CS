package meta;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.meta.*;
import edu.sustech.cs307.value.ValueType;
import org.junit.jupiter.api.*;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;


class MetaManagerTest {
    private MetaManager metaManager;
    private static final String TEST_META_FILE = "meta_data.json";

    @TempDir
    private Path root_dir;


    @BeforeEach
    void setUp() throws DBException {
        metaManager = new MetaManager(root_dir.toString());
    }

    @AfterEach
    void tearDown() {
        File file = new File(TEST_META_FILE);
        if (file.exists()) {
            file.delete();
        }
    }

    @Test
    void testCreateAndRetrieveTable() throws DBException {
        TableMeta tableMeta = new TableMeta("users");
        tableMeta.addColumn(new ColumnMeta("users","id", ValueType.INTEGER, 0, 4));
        tableMeta.addColumn(new ColumnMeta("users", "name", ValueType.CHAR, 4, 4));

        metaManager.createTable(tableMeta);
        TableMeta retrievedMeta = metaManager.getTable("users");

        assertNotNull(retrievedMeta);
        assertEquals("users", retrievedMeta.tableName);
        assertTrue(retrievedMeta.getColumns().containsKey("id"));
        assertTrue(retrievedMeta.getColumns().containsKey("name"));
    }

    @Test
    void testCreateTableWithNoColumnsShouldFail() {
        TableMeta emptyTable = new TableMeta("emptyTable");
        assertThrows(DBException.class, () -> metaManager.createTable(emptyTable));
    }

    @Test
    void testDropTable() throws DBException {
        TableMeta tableMeta = new TableMeta("orders");
        tableMeta.addColumn(new ColumnMeta("orders", "order_id", ValueType.INTEGER, 0, 4));
        metaManager.createTable(tableMeta);
        metaManager.dropTable("orders");
        assertThrows(DBException.class, () -> metaManager.getTable("orders"));
    }

    @Test
    void testAddAndDropColumn() throws DBException {
        TableMeta tableMeta = new TableMeta("products");
        tableMeta.addColumn(new ColumnMeta("products", "product_id", ValueType.INTEGER, 0, 4));
        metaManager.createTable(tableMeta);

        metaManager.addColumnInTable("products", new ColumnMeta("products", "price", ValueType.FLOAT, 0, 4));
        assertTrue(metaManager.getTable("products").getColumns().containsKey("price"));

        metaManager.dropColumnInTable("products", "price");
        assertFalse(metaManager.getTable("products").getColumns().containsKey("price"));
    }

    @Test
    void testTableAlreadyExistsException() throws DBException {
        TableMeta tableMeta = new TableMeta("customers");
        tableMeta.addColumn(new ColumnMeta("customer_id", "customers", ValueType.INTEGER, 0, 4));
        metaManager.createTable(tableMeta);
        assertThrows(DBException.class, () -> metaManager.createTable(tableMeta));
    }

    @Test
    void testTableDoesNotExistException() {
        assertThrows(DBException.class, () -> metaManager.dropTable("non_existent_table"));
    }

    @Test
    void testColumnAlreadyExistsException() throws DBException {
        TableMeta tableMeta = new TableMeta("employees");
        tableMeta.addColumn(new ColumnMeta("salary", "employees", ValueType.FLOAT, 0, 4));
        metaManager.createTable(tableMeta);
        ColumnMeta column = new ColumnMeta("salary", "employees", ValueType.FLOAT, 0, 4);
        assertThrows(DBException.class, () -> metaManager.addColumnInTable("employees", column));
    }
}
