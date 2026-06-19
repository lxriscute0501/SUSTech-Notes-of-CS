package edu.sustech.cs307.storage.replacer;

public interface PageReplacer {
    int Victim();

    void Pin(int frameId);

    void Unpin(int frameId);

    int size();
}
