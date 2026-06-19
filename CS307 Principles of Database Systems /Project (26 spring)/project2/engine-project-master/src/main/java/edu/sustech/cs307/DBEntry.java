package edu.sustech.cs307;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.logicalOperator.LogicalOperator;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.meta.MetaManager;
import edu.sustech.cs307.optimizer.LogicalPlanner;
import edu.sustech.cs307.optimizer.PhysicalPlanner;
import edu.sustech.cs307.physicalOperator.PhysicalOperator;
import edu.sustech.cs307.storage.BufferPool;
import edu.sustech.cs307.storage.DiskManager;
import edu.sustech.cs307.system.DBManager;
import edu.sustech.cs307.system.RecordManager;
import edu.sustech.cs307.system.TransactionManager;
import edu.sustech.cs307.tuple.Tuple;

import org.apache.commons.lang3.StringUtils;
import org.jline.reader.LineReader;
import org.jline.reader.LineReaderBuilder;
import org.jline.terminal.TerminalBuilder;
import org.pmw.tinylog.Logger;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class DBEntry {
    public static final String DB_NAME = "CS307-DB";
    // for now, we use 256 * 512 * 4096 bytes = 512MB as the pool size
    public static final int POOL_SIZE = 256 * 512;

    public static void printHelp() {
        Logger.info("Type 'exit' to exit the program.");
        Logger.info("Type 'help' to see this message again.");
    }

    public static void main(String[] args) throws DBException {
        Logger.getConfiguration().formatPattern("{date: HH:mm:ss.SSS} {level}: {message}").activate();

        Logger.info("Hello, This is CS307-DB!");
        Logger.info("Initializing...");
        DBManager dbManager = null;
        try {
            Map<String, Integer> disk_manager_meta = new HashMap<>(DiskManager.read_disk_manager_meta());
            DiskManager diskManager = new DiskManager(DB_NAME, disk_manager_meta);
            BufferPool bufferPool = new BufferPool(POOL_SIZE, diskManager);
            RecordManager recordManager = new RecordManager(diskManager, bufferPool);
            MetaManager metaManager = new MetaManager(DB_NAME + "/meta");
            dbManager = new DBManager(diskManager, bufferPool, recordManager, metaManager);
        } catch (DBException e) {
            Logger.error(e.getMessage());
            Logger.error("An error occurred during initializing. Exiting....");
            return;
        }

        String sql = "";
        boolean running = true;
        try {
            while (running) {
                try {
                    LineReader scanner = LineReaderBuilder.builder()
                            .terminal(
                                    TerminalBuilder
                                            .builder()
                                            .dumb(true)
                                            .build()
                            )
                            .build();
                    Logger.info("CS307-DB> ");
                    sql = scanner.readLine();
                    if (sql.equalsIgnoreCase("exit")) {
                        running = false;
                        continue;
                    } else if (sql.equalsIgnoreCase("help")) {
                        printHelp();
                        continue;
                    }
                } catch (Exception e) {
                    Logger.error(e.getMessage());
                    Logger.error("An error occurred. Exiting....");
                }
                try {
                    LogicalOperator operator = LogicalPlanner.resolveAndPlan(dbManager, sql);
                    if (operator == null) {
                        continue;
                    }
                    PhysicalOperator physicalOperator = PhysicalPlanner.generateOperator(dbManager, operator);
                    if (physicalOperator == null) {
                        Logger.info(operator);
                        continue;
                    }
                    Logger.info(getStartEndLine(physicalOperator.outputSchema().size(), true));
                    Logger.info(getHeaderString(physicalOperator.outputSchema()));
                    Logger.info(getSperator(physicalOperator.outputSchema().size()));
                    physicalOperator.Begin();
                    while (physicalOperator.hasNext()) {
                        physicalOperator.Next();
                        Tuple tuple = physicalOperator.Current();
                        Logger.info(getRecordString(tuple));
                        Logger.info(getSperator(physicalOperator.outputSchema().size()));
                    }
                    physicalOperator.Close();
                    dbManager.getBufferPool().FlushAllPages("");
                } catch (DBException e) {
                    Logger.error(e.getMessage());
                    Logger.error("An error occurred. Please try again.");
                    Logger.error(Arrays.toString(e.getStackTrace()));
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            // persist the disk manager
            dbManager.getBufferPool().FlushAllPages("");
            Logger.error("Some error occurred. Exiting after persistdata...");
        }
    }

    private static String getHeaderString(ArrayList<ColumnMeta> columnMetas) {
        StringBuilder header = new StringBuilder("|");
        for (var entry : columnMetas) {
            String tabcol = String.format("%s.%s", entry.tableName, entry.name);
            String centeredText = StringUtils.center(tabcol, 15, ' ');
            header.append(centeredText).append("|");
        }
        return header.toString();
    }

    private static String getRecordString(Tuple tuple) throws DBException {
        StringBuilder tuple_string = new StringBuilder("|");
        for (var entry : tuple.getValues()) {
            String tabCol = String.format("%s", entry);
            String centeredText = StringUtils.center(tabCol, 15, ' ');
            tuple_string.append(centeredText).append("|");
        }
        return tuple_string.toString();
    }

    private static String getSperator(int width) {
        // ───────────────
        StringBuilder line = new StringBuilder("+");
        for (int i = 0; i < width; i++) {
            line.append("───────────────");
            line.append("+");
        }
        return line.toString();
    }

    private static String getStartEndLine(int width, boolean header) {
        StringBuilder end_line;
        if (header) {
            end_line = new StringBuilder("┌");
        } else {
            end_line = new StringBuilder("└");
        }
        for (int i = 0; i < width; i++) {
            end_line.append("───────────────");
            if (header) {
                end_line.append("┐");
            } else {
                end_line.append("┘");
            }
        }
        return end_line.toString();
    }
}
