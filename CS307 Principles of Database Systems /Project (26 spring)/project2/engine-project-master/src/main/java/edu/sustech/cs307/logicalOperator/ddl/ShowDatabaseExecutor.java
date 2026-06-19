package edu.sustech.cs307.logicalOperator.ddl;

import edu.sustech.cs307.exception.DBException;
import edu.sustech.cs307.exception.ExceptionTypes;
import net.sf.jsqlparser.statement.ShowStatement;
import org.pmw.tinylog.Logger;

public class ShowDatabaseExecutor implements DMLExecutor {

    ShowStatement showStatement;
    public ShowDatabaseExecutor(ShowStatement showStatement) {
        this.showStatement = showStatement;
    }
    @Override
    public void execute() throws DBException {
        String command = showStatement.getName();
        if (command.equalsIgnoreCase("DATABASES")) {
            // we only have one database
            Logger.info("|-----------|");
            Logger.info("| Databases |");
            Logger.info("|-----------|");
            Logger.info("|   CS307   |");
            Logger.info("|-----------|");
        } else {
            throw new DBException(ExceptionTypes.UnsupportedCommand(String.format("SHOW %s", command)));
        }
    }

}
