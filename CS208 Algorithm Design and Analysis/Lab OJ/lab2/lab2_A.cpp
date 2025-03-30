#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<vector<int>> g, gr;
vector<long long> ans, ansr;

vector<long long> bfs(vector<vector<int>> g, int n, int m) {
    vector<int> in(n + 1, 0);
    vector<long long> chains(n + 1);
    queue<int> q;
    
    for (int i = 1; i <= n; i++)
        chains[i] = 0;
    
    for (int i = 1; i <= n; i++)
    {
        for (int v : g[i])
        in[v] ++;
    }
        
    for (int u = 1; u <= n; u++)
        if (in[u] == 0) {
            q.push(u);
            chains[u] = 1;
        }
        
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : g[u])
        {
            in[v] --;
            chains[v] = (chains[v] + chains[u]) % 1000000007;
            if (in[v] == 0) q.push(v);
        }
    }
        
    return chains;
}

int main()
{
    int n, m;
    cin >> n >> m;
    
    g.resize(n + 1); gr.resize(n + 1);
    ans.resize(n + 1); ansr.resize(n + 1);
    
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        gr[v].push_back(u);
    }
    
    ans = bfs(g, n, m);
    ansr = bfs(gr, n, m);
    
    for (int i = 1; i <= n; i++)
        cout << (ans[i] * ansr[i]) % 1000000007 << " ";
    return 0;
}
