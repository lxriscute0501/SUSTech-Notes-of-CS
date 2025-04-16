#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int u, v, w;
};

vector<long long> dijkstra(int start, vector<vector<pair<long long, long long>>>& g) {
    int n = g.size();
    vector<long long> dist(n, LLONG_MAX);
    dist[start] = 0;
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<pair<long long, long long>>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        long long d = pq.top().first, u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;

        for (auto& edge : g[u])
        {
            long long v = edge.first, w = edge.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<pair<long long, long long>>> g(n + 1);
    vector<Edge> edges(m);
    
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
        edges[i] = {u, v, w};
    }

    vector<long long> d1 = dijkstra(1, g);
    vector<long long> dn = dijkstra(n, g);

    for (int i = 1; i <= n; i++)
    {
        long long sum = d1[i] + dn[i];
        cout << sum << "\n";
    }

    for (int i = 0; i < m; i++)
    {
        long long minn = min(d1[edges[i].u] + edges[i].w + dn[edges[i].v], d1[edges[i].v] + edges[i].w + dn[edges[i].u]);
        
        cout << minn << "\n";
    }

    return 0;
}