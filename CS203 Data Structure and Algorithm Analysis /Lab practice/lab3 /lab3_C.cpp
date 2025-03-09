#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

struct Node {
    int value;
    Node* prev;
    Node* next;
    Node(int v) : value(v), prev(nullptr), next(nullptr) {}
};

class LinkedList {
public:
    unordered_map<int, Node*> posMap;
    Node* head;
    Node* tail;

    LinkedList() : head(nullptr), tail(nullptr) {}

    void push_back(int value) {
        Node* newNode = new Node(value);
        posMap[value] = newNode;

        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void swapRanges(int x1, int y1, int x2, int y2)
    {
        Node* start1 = posMap[x1];
        Node* end1 = posMap[y1];
        Node* start2 = posMap[x2];
        Node* end2 = posMap[y2];
        
        Node* start1_prev = start1->prev;
        Node* end1_next = end1->next;
        Node* start2_prev = start2->prev;
        Node* end2_next = end2->next;

        if (start1_prev) start1_prev->next = start2;
        if (end1_next) end1_next->prev = end2;
        if (start2_prev) start2_prev->next = start1;
        if (end2_next) end2_next->prev = end1;

        Node* temp_start = start1->prev;
        Node* temp_end = end1->next;
        start1->prev = start2->prev;
        end1->next = end2->next;
        start2->prev = temp_start;
        end2->next = temp_end;

        if (!start1->prev) head = start1;
        if (!start2->prev) head = start2;
        if (!end1->next) tail = end1;
        if (!end2->next) tail = end2;
    }

    void printList() {
        Node* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main() {
    int t;
    cin>>t;
    for (int j=1; j<=t; j++)
    {
        int n, m;
        cin >>n>>m;

        LinkedList linkedList;
        for (int i=0; i<n; i++)
        {
            int val;
            cin >> val;
            linkedList.push_back(val);
        }

        for (int i=0; i<m; i++)
        {
            int x1, y1, x2, y2;
            cin>>x1>>y1>>x2>>y2;
            linkedList.swapRanges(x1, y1, x2, y2);
        }

        linkedList.printList();
    }

    return 0;
}
