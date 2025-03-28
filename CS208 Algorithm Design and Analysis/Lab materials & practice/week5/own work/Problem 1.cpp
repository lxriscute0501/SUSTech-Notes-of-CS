#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> g(n + 1);
    vector<int> in(n + 1, 0);
    
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        in[v] ++;
    }
    
    priority_queue<int, vector<int>, greater<int>> pq;
    
    for (int i = 1; i <= n; i++)
    {
        if (in[i] == 0) pq.push(i);
    }
    
    vector<int> result;
    while (!pq.empty())
    {
        int u = pq.top();
        pq.pop();
        result.push_back(u);
        
        for (int v : g[u])
        {
            in[v] --;
            if (in[v] == 0) pq.push(v);
        }
    }
    
    if (result.size() != n) cout << "impossible" << endl;
    else {
        for (int i = 0; i < n; i++)
            cout << result[i];
    }
    
    return 0;
}
