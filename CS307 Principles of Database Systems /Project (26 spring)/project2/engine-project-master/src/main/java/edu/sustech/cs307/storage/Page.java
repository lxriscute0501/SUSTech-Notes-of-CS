package edu.sustech.cs307.storage;


import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;

public class Page {
    public final static int DEFAULT_PAGE_SIZE = 4 * 1024;

    public final static int PAGE_HEADER_SIZE = 8;

    public ByteBuf data;
    public PagePosition position = new PagePosition("null", 0);
    public boolean dirty;
    public int pin_count = 0;

    public int getPageID() {
        return position.offset / DEFAULT_PAGE_SIZE;
    }

    public Page() {
        data = Unpooled.buffer(DEFAULT_PAGE_SIZE);
    }
}
