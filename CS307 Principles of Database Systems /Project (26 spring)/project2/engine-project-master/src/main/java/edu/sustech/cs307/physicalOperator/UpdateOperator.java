package edu.sustech.cs307.physicalOperator;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import edu.sustech.cs307.meta.ColumnMeta;
import edu.sustech.cs307.meta.TabCol;
import edu.sustech.cs307.record.RecordFileHandle;
import edu.sustech.cs307.tuple.TableTuple;
import edu.sustech.cs307.tuple.TempTuple;
import edu.sustech.cs307.tuple.Tuple;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;

import edu.sustech.cs307.value.Value;
import edu.sustech.cs307.value.ValueType;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.statement.update.UpdateSet;

public class UpdateOperator implements PhysicalOperator {
    private final SeqScanOperator seqScanOperator;
    private final String tableName;
    private final UpdateSet updateSet;
    private final Expression whereExpr;

    private int updateCount;
    private boolean isDone;

    public UpdateOperator(PhysicalOperator inputOperator, String tableName, UpdateSet updateSet,
                          Expression whereExpr) {
        if (!(inputOperator instanceof SeqScanOperator seqScanOperator)) {
            throw new RuntimeException("The delete operator only accepts SeqScanOperator as input");
        }
        this.seqScanOperator = seqScanOperator;
        this.tableName = tableName;
        this.updateSet = updateSet;
        this.whereExpr = whereExpr;
        this.updateCount = 0;
        this.isDone = false;
    }

    @Override
    public boolean hasNext() {
        return !isDone;
    }

    @Override
    public void Begin() throws DBException {
        seqScanOperator.Begin();
        RecordFileHandle fileHandle = seqScanOperator.getFileHandle();

        while (seqScanOperator.hasNext()) {
            seqScanOperator.Next();
            TableTuple tuple = (TableTuple) seqScanOperator.Current();

            if (whereExpr == null || tuple.eval_expr(whereExpr)) {
                Value[] oldValues = tuple.getValues();
                List<Value> newValues = new ArrayList<>(Arrays.asList(oldValues));
                TabCol[] schema = tuple.getTupleSchema();

                for (int i = 0; i < this.updateSet.getColumns().size(); i++) {
                    String targetTable = updateSet.getColumn(i).getTableName();
                    if (targetTable == null) targetTable = tuple.getTableName();
                    String targetColumn = updateSet.getColumn(i).getColumnName();
                    int index = -1;
                    for (int j = 0; j < schema.length; j++) {
                        if (schema[j].getColumnName().equalsIgnoreCase(targetColumn)
                                && schema[j].getTableName().equalsIgnoreCase(targetTable)) {
                            index = j;
                            break;
                        }
                    }
                    if (index == -1) {
                        throw new DBException(ExceptionTypes.ColumnDoesNotExist(targetColumn));
                    }
                    Value newValue = tuple.evaluateExpression(updateSet.getValue(i));
                    newValues.set(index, newValue);
                }
                ByteBuf buffer = Unpooled.buffer();
                for (Value v : newValues) {
                    String str = "";
                    if (v.type == ValueType.CHAR) str = (String) v.value;
                    if (str.length() == 64) {
                        ByteBuffer temp = ByteBuffer.allocate(64);
                        temp.put(str.getBytes());
                        buffer.writeBytes(temp.array());
                    }
                    else buffer.writeBytes(v.ToByte());
                }

                fileHandle.UpdateRecord(tuple.getRID(), buffer);
                updateCount++;
            }
        }
    }

    @Override
    public void Next() {
        isDone = true;
    }

    @Override
    public Tuple Current() {
        if (isDone) {
            ArrayList<Value> result = new ArrayList<>();
            result.add(new Value(updateCount, ValueType.INTEGER));
            return new TempTuple(result);
        } else {
            throw new RuntimeException("Call Next() first");
        }
    }

    @Override
    public void Close() {
        seqScanOperator.Close();
    }

    @Override
    public ArrayList<ColumnMeta> outputSchema() {
        ArrayList<ColumnMeta> schema = new ArrayList<>();
        schema.add(new ColumnMeta("update", "numberOfUpdatedRows", ValueType.INTEGER, 0, 0));
        return schema;
    }

    public void reset() {
        updateCount = 0;
        isDone = false;
    }

    public Tuple getNextTuple() {
        if (hasNext()) {
            Next();
            return Current();
        }
        return null;
    }

    public void close() {
        Close();
    }

    public String getTableName() {
        return tableName;
    }
}
