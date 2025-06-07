#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

vector<int> adj[601];
int cap[601][601];
int parent[601];
int n, m, s, t;

bool bfs() {
    memset(parent, -1, sizeof(parent));
    queue<int> q;
    q.push(s);
    parent[s] = -2;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u])
        {
            if (parent[v] == -1 and cap[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int maxFlow() {
    int res = 0;
    while (bfs()) {
        int flow = INT_MAX;
        
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            flow = min(flow, cap[u][v]);
        }
        
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            cap[u][v] -= flow;
            cap[v][u] += flow;
        }
        res += flow;
    }
    return res;
}

int main()
{
    cin >> n >> m >> s >> t;
    for (int i = 1; i <= n; i++)
    {
        if (i == s or i == t) {
            cap[i][i+n] = INT_MAX;
        } else {
            cap[i][i+n] = 1;
        }
        adj[i].push_back(i+n);
        adj[i+n].push_back(i);
    }

    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        cap[u+n][v] = cap[v+n][u] = INT_MAX;
        adj[u+n].push_back(v);
        adj[v+n].push_back(u);
    }

    cout << maxFlow();
    return 0;
}
