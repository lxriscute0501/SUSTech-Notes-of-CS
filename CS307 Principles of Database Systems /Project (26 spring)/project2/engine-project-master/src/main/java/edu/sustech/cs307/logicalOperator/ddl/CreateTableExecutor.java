package edu.sustech.cs307.logicalOperator.ddl;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.system.DBManager;
import edu.sustech.cs307.value.Value;
import edu.sustech.cs307.value.ValueType;
import net.sf.jsqlparser.statement.create.table.CreateTable;
import net.sf.jsqlparser.statement.create.table.ColDataType;
import org.pmw.tinylog.Logger;

import java.util.ArrayList;

public class CreateTableExecutor implements DMLExecutor {
    // Logger Logger = LoggerFactory.getLogger(CreateTableExecutor.class); //
    // Removed SLF4j LoggerFactory

    private final CreateTable createTableStmt;
    private final DBManager dbManager;
    private final String sql;

    public CreateTableExecutor(CreateTable createTable, DBManager dbManager, String sql) {
        this.createTableStmt = createTable;
        this.dbManager = dbManager;
        this.sql = sql;
    }

    @Override
    public void execute() throws DBException {
        String table = createTableStmt.getTable().getName();
        ArrayList<ColumnMeta> colMapping = new ArrayList<>();
        int offset = 0;
        if (null == createTableStmt.getColumnDefinitions()) {
            throw new DBException(ExceptionTypes.TableHasNoColumn(table));
        }
        for (var col : createTableStmt.getColumnDefinitions()) {
            // transform the column definition to ColumnMeta
            // we only accept the char, int, float type
            String colName = col.getColumnName();
            if (colName.isEmpty() || colName.length() > 10) {
                throw new DBException(
                        ExceptionTypes.InvalidSQL(sql, String.format("INVALID COLUMN NAME = %s", colName)));
            }
            ColDataType colType = col.getColDataType();
            if (colType.getDataType().equalsIgnoreCase("char")) {
                colMapping.add(new ColumnMeta(table, colName, ValueType.CHAR, Value.CHAR_SIZE, offset));
                offset += Value.CHAR_SIZE;
            } else if (colType.getDataType().equalsIgnoreCase("int")) {
                colMapping.add(new ColumnMeta(table, colName, ValueType.INTEGER, Value.INT_SIZE, offset));
                offset += Value.INT_SIZE;
            } else if (colType.getDataType().equalsIgnoreCase("float")) {
                colMapping.add(new ColumnMeta(table, colName, ValueType.FLOAT, Value.FLOAT_SIZE, offset));
                offset += Value.FLOAT_SIZE;
            } else {
                throw new DBException(ExceptionTypes.UnsupportedCommand(String.format("CREATE TABLE %s", table)));
            }
        }
        dbManager.createTable(table, colMapping);
        Logger.info("Successfully created table: {}", table); // Modified to Tinylog format
    }

}
