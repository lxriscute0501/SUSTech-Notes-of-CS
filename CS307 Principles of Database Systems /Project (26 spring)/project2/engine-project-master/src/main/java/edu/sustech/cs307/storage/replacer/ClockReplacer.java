package edu.sustech.cs307.storage.replacer;

import java.util.List;

public class ClockReplacer implements PageReplacer{
    private List<Integer> frames;

    public ClockReplacer(int numPages) {
    }

    @Override
    public int Victim() {
        return 0;
    }

    @Override
    public void Pin(int frameId) {

    }

    @Override
    public void Unpin(int frameId) {

    }

    @Override
    public int size() {
        return frames.size();
    }
}
