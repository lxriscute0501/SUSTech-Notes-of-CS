package edu.sustech.cs307.meta;

public class TabCol {
    private String tableName;
    private String columnName;

    public TabCol(String tableName, String columnName) {
        this.tableName = tableName;
        this.columnName = columnName;
    }

    public String getTableName() {
        return tableName;
    }

    public String getColumnName() {
        return columnName;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        TabCol tabCol = (TabCol) obj;
        return tableName.equals(tabCol.tableName) && columnName.equals(tabCol.columnName);
    }

    @Override
    public int hashCode() {
        int result = tableName.hashCode();
        result = 31 * result + columnName.hashCode();
        return result;
    }
}
