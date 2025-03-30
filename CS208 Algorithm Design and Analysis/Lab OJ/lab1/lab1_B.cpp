#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<vector<int>> graph;

long long bfs(int n, int start, int stop) {
    vector<bool> visited(n + 1, true);
    queue<int> q;
    long long ans = 0;
    
    q.push(start);
    visited[start] = false;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u])
        {
            if (v != stop and visited[v]) {
                q.push(v);
                visited[v] = false;
            }
        }
    }

    for (int i = 1; i <= n; i++)
    if (i != stop and visited[i]) ans ++;
    
    return ans;
}

int main()
{
    int n, m, a, b;
    cin >> n >> m >> a >> b;

    graph.resize(n + 1);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    long long ans = bfs(n, b, a) * bfs(n, a, b);
    cout << ans;

    return 0;
}
