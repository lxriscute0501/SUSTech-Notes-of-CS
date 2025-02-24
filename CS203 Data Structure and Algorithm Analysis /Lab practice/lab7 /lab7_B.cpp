#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> heap;
    vector<bool> bo;
    
    void weihu(int idx)
    {
        while (idx > 0 and heap[idx] > heap[(idx - 1) / 2])
        {
            swap(heap[idx], heap[(idx - 1) / 2]);
            swap(bo[idx], bo[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }
    
public:
    void insert(int val, bool b)
    {
        heap.push_back(val);
        bo.push_back(b);
        weihu(heap.size() - 1);
    }
    
    void search()
    {
        for (int i=0; i<bo.size(); i++)
        {
            if (bo[i]) {
                int s = 1, ans = 1;
                int x = 1, y = 1;
                if (i == 0) {
                    cout << x << " " << y << endl;
                    return;
                }
                while (ans + s * 2 < i+1) {
                    s *= 2;
                    ans += s;
                    x ++;
                }
                x ++;
                y = i+1 - ans;
                cout << x << " " << y << endl;
                return;
            }

        }
    }
    
};

int main()
{
    int T, n, ind;
    cin >> T;
    while (T --)
    {
        cin >> n;
        int a[n+1];
        bool b[n+1];
        
        for (int i=1; i<=n; i++)
            cin >> a[i];
        for (int i=1; i<=n; i++)
            b[i] = false;
        
        cin >> ind;
        b[ind] = true;
        
        MaxHeap maxheap;
        
        for (int i=1; i<=n; i++)
            maxheap.insert(a[i], b[i]);
        
        maxheap.search();
    }
    return 0;
}
