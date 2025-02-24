#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Node {
public:
    Node* prev = nullptr;
    Node* next = nullptr;
    int value;
    int idx;
    bool life = true;
};

class Heap {
public:
    vector<Node*> heap;
    int num = 0;
    int lifes = 0;

    Heap(int N) {
        heap.resize(N + 1);
    }

    void insert(Node* x)
    {
        heap[++num] = x;
        lifes ++;
        int k = num;
        
        while (k > 1)
        {
            if (heap[k]->value < heap[k/2]->value or
               (heap[k]->value == heap[k/2]->value and heap[k]->idx > heap[k/2]->idx))
            {
                swap(heap[k], heap[k/2]);
                k /= 2;
            } else break;
        }
    }

    void weihu_down()
    {
        int k = 1;
        while (2*k <= num)
        {
            if (2*k+1 <= num) {
                if (heap[k]->value > heap[2*k]->value or heap[2*k+1]->value < heap[k]->value) {
                    if (heap[2*k]->value < heap[2*k+1]->value or
                       (heap[2*k]->idx < heap[2*k+1]->idx and heap[2*k+1]->value == heap[2*k]->value)) {
                        swap(heap[k], heap[2*k]);
                        k *= 2;
                    } else {
                        swap(heap[k], heap[2*k+1]);
                        k = 2*k+1;
                    }
                } else return;
            } else {
                if (heap[k]->value > heap[2*k]->value or
                   (heap[k]->value == heap[2*k]->value and heap[k]->idx > heap[2*k]->idx)) {
                    swap(heap[k], heap[2*k]);
                }
                return;
            }
        }
    }

    void merge()
    {
        if (!heap[1]->life)
        {
            heap[1] = heap[num --];
            weihu_down();
            return;
        }
        if (lifes == 1)
        {
            cout << heap[1]->value << endl;
            num = 0;
            return;
        }
        
        Node* p;
        int x = heap[1]->value;

        if (!heap[1]->prev->life)
        {
            p = heap[1]->next;
            heap[1]->value = (x ^ p->value) + 1;
            heap[1]->next = p->next;
            p->next->prev = heap[1];
            p->life = false;
        } else if (!heap[1]->next->life) {
            p = heap[1]->prev;
            heap[1]->value = (x ^ p->value) + 1;
            heap[1]->prev = p->prev;
            p->prev->next = heap[1];
            p->life = false;
        } else {
            int l = (x ^ heap[1]->prev->value) + 1;
            int n = (x ^ heap[1]->next->value) + 1;
            if (l <= n) {
                p = heap[1]->next;
                heap[1]->value = (x ^ p->value) + 1;
                heap[1]->next = p->next;
                p->next->prev = heap[1];
            } else {
                p = heap[1]->prev;
                heap[1]->value = (x ^ p->value) + 1;
                heap[1]->prev = p->prev;
                p->prev->next = heap[1];
            }
            p->life = false;
        }
        lifes --;
        weihu_down();
    }
};

int main()
{
    int N;
    cin >> N;
    
    vector<Node*> arr(N + 2);
    for (int i = 0; i < N + 2; i++)
    {
        arr[i] = new Node();
        arr[i]->idx = i;
        if (i == N+1 or i == 0) arr[i]->life = false;
        else cin >> arr[i]->value;
    }

    Node* front = arr[0];
    Node* bott = arr[N + 1];
    for (int i = 1; i < N + 1; i++)
    {
        arr[i]->prev = arr[i-1];
        arr[i]->next = arr[i+1];
    }
    front->next = arr[1];
    bott->prev = arr[N];

    Heap h(N);
    for (int i = 1; i <= N; i++)
        h.insert(arr[i]);
    while (h.num != 0) h.merge();

    return 0;
}
