#include <iostream>
#include <vector>
using namespace std;

class minheap {
private:
    vector<int> heap;

    void weihu(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (heap[parent] <= heap[index]) break;
            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void findmin(int index)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < heap.size() and heap[leftChild] < heap[smallest])
            smallest = leftChild;
        if (rightChild < heap.size() and heap[rightChild] < heap[smallest])
            smallest = rightChild;

        if (smallest != index) {
            swap(heap[smallest], heap[index]);
            findmin(smallest);
        }
    }

public:
    void insert(int value)
    {
        heap.push_back(value);
        weihu(heap.size() - 1);
    }

    int removemin()
    {
        if (heap.size() == 0) return -1;
        int minValue = heap[0];
        
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        findmin(0);
        
        return minValue;
    }

    int size() const {
        return heap.size();
    }
};

int minimalCutCost(int N, vector<int>& a)
{
    minheap minHeap;
    
    for (int i = 0; i < N; i++)
        minHeap.insert(a[i]);

    int totalCost = 0;

    while (minHeap.size() > 1)
    {
        int news = 0;
        news += minHeap.removemin();
        news += minHeap.removemin();
        
        totalCost += news;
        
        minHeap.insert(news);
    }
    return totalCost;
}

int main()
{
    int T;
    cin >> T;

    while (T --)
    {
        int N;
        cin >> N;
        vector<int> a(N);

        for (int i=0; i<N; i++)
            cin >> a[i];
        cout << minimalCutCost(N, a) << endl;
    }

    return 0;
}
