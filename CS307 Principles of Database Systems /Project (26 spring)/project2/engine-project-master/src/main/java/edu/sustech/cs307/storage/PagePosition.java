package edu.sustech.cs307.storage;

public class PagePosition {
    public String filename;
    public int offset;

    public PagePosition(String filename, int offset){
        this.filename = filename;
        this.offset = offset;
    }

    @Override
    public int hashCode() {
        return (filename.hashCode() << 16) ^ offset;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof PagePosition other) {
            return filename.equals(other.filename) && offset == other.offset;
        }
        return false;
    }
}
