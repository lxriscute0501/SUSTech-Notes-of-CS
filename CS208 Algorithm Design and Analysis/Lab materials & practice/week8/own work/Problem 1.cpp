#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int mincost(vector<int>& piles) {
    priority_queue<int, vector<int>, greater<int>> minheap;
    for (int pile : piles)
        minheap.push(pile);
    
    int sum = 0;
    while (minheap.size() > 1)
    {
        int x = minheap.top();
        minheap.pop();
        int y = minheap.top();
        minheap.pop();
        
        sum += x + y;
        minheap.push(x + y);
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
    int n;
    cin >> n;
    vector<int> piles(n);
    for (int i = 0; i < n; i++)
        cin >> piles[i];
        
    int minn = mincost(piles);
    int maxx = maxcost(piles);
    cout << minn << " " << maxx;
    return 0;
}