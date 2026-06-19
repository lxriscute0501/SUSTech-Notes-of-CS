package edu.sustech.cs307.meta;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import edu.sustech.cs307.value.*;


public class ColumnMeta {

    @JsonProperty("name")
    public String name;

    @JsonProperty("type")
    public ValueType type;

    @JsonProperty("len")
    public int len;

    @JsonProperty("offset")
    public int offset;

    @JsonProperty("tableName")
    public String tableName;


    @JsonCreator
    public ColumnMeta(@JsonProperty("tableName") String tableName,
                      @JsonProperty("name") String name,
                      @JsonProperty("type") ValueType type,
                      @JsonProperty("len") int len,
                      @JsonProperty("offset") int offset) {
        this.tableName = tableName;
        this.name = name;
        this.type = type;
        this.len = len;
        this.offset = offset;
    }

    public int getLen() {
        return len;
    }

    public int getOffset() {
        return offset;
    }
}
