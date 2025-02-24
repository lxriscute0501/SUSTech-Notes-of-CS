#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m, k, c;
int color[100001];
vector<int> g[100001];
int dist[100001][101];

void bfs(int x)
{
    int front = 0, rear = 0;
    int q[100001];
    bool visited[100001];
    
    for (int i = 0; i <= n; i++)
        visited[i] = false;
    
    for (int i = 1; i <= n; i++)
    {
        if (color[i] == x) {
            visited[i] = true;
            q[rear++] = i;
            dist[i][x] = 0;
        }
    }

    while (front < rear)
    {
        int u = q[front++];
        for (int i=0; i<g[u].size(); i++)
        {
            int v = g[u][i];
            if (!visited[v]) {
                visited[v] = true;
                q[rear++] = v;
                dist[v][x] = dist[u][x]+1;
            }
        }
    }
}

int main()
{
    cin >> n >> m >> k >> c;
    for (int i = 1; i <= n; i++)
        cin >> color[i];

    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= k; j++)
            dist[i][j] = 1e9;

    for (int i = 1; i <= k; i++)
        bfs(i);

    for (int t = 1; t <= n; t++)
    {
        vector<int> costs;
        for (int i = 1; i <= k; i++)
        {
            if (dist[t][i] < 1e9) costs.push_back(dist[t][i]);
        }

        sort(costs.begin(), costs.end());
        long long sum = 0;
        int count = 0, minn;
        
        if (costs.size() > c) minn = c; else minn = costs.size();
        for (int i = 0; i < minn; i++)
        {
            sum += costs[i];
            count ++;
        }
        if (count < c) cout << -1 << " "; else cout << sum << " ";
    }
    return 0;
}
