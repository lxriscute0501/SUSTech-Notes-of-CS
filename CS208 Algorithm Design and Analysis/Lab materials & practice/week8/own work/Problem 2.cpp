#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int mincost(vector<int>& piles, int n, int k) {
    priority_queue<int, vector<int>, greater<int>> minheap;
    for (int pile : piles)
        minheap.push(pile);
    
    int sum = 0, x = (n - 1) % (k - 1) + 1, mergecost = 0;
    if (x > 1) {
        for (int i = 0; i < x; i++)
        {
            mergecost += minheap.top();
            minheap.pop();
        }
        minheap.push(mergecost);
        sum += mergecost;
    }
    
    while (minheap.size() > 1)
    {
        mergecost = 0;
        for (int i = 0; i < k; i++)
        {
            mergecost += minheap.top();
            minheap.pop();
        }
        minheap.push(mergecost);
        sum += mergecost;
    }
    
    return sum;
}

int maxcost(vector<int>& piles) {
    priority_queue<int, vector<int>, less<int>> maxheap;
    for (int pile : piles)
        maxheap.push(pile);
    
    int sum = 0;
    while (maxheap.size() > 1)
    {
        int x = maxheap.top();
        maxheap.pop();
        int y = maxheap.top();
        maxheap.pop();
        
        sum += x + y;
        maxheap.push(x + y);
    }
    
    return sum;
}

int main()
{
    int T;
    cin >> T;
    while (T--)
    {
        int n, k;
        cin >> n >> k;
        vector<int> piles(n);
        for (int i = 0; i < n; i++)
            cin >> piles[i];
        
        int minn = mincost(piles, n, k);
        int maxx = maxcost(piles);
        cout << maxx << " " << minn << endl;
    }
    
    return 0;
}
