#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    int key, value;
    Node* next;
    Node* prev;
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRU {
private:
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head = new Node(-1, -1);
    Node* tail = new Node(-1, -1);

    void add(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void remove(Node* node) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

public:
    LRU(int c) {
        capacity = c;
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        Node* node = cache[key];
        remove(node);
        add(node);
        return node->value;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->value = value;
            remove(node);
            add(node);
        } else {
            Node* node = new Node(key, value);
            cache[key] = node;
            add(node);
            if (cache.size() > capacity) {
                Node* last = tail->prev;
                cache.erase(last->key);
                remove(tail->prev);
            }
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m;
    cin >> n >> m;

    LRU lru(n);

    for (int i = 1; i <= m; i++)
    {
        string s;
        cin >> s;
        if (s == "get") {
            int k;
            cin >> k;
            cout << lru.get(k) << '\n';
        }
        if (s == "put") {
            int k, v;
            cin >> k >> v;
            lru.put(k, v);
        }
    }

    return 0;
}
