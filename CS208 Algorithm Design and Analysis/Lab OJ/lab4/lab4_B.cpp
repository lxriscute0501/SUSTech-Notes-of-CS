#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

int n, m;

vector<long long> dijkstra(vector<vector<pair<long long, int>>>& g) {
    vector<long long> dist(n + 1, LLONG_MAX);
    dist[1] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, 1});

    while (!pq.empty())
    {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;

        for (auto& edge : g[u])
        {
            long long w = edge.first;
            int v = edge.second;
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
    cin >> n >> m;
    vector<vector<pair<long long, int>>> g(n + 1);
    vector<int> u(m), v(m);
    vector<long long> w(m);

    for (int i = 0; i < m; i++)
    {
        cin >> u[i] >> v[i] >> w[i];
        g[u[i]].push_back({w[i], v[i]});
    }

    vector<long long> d = dijkstra(g);

    vector<long long> minn(n + 1, LLONG_MAX);
    for (int i = 0; i < m; i++)
    {
        if (d[u[i]] + w[i] == d[v[i]]) minn[v[i]] = min(minn[v[i]], w[i]);
    }
    
    long long sum = 0;
    for (int i = 2; i <= n; i++)
    {
        if (minn[i] != LLONG_MAX) sum += minn[i];
        else {
            sum = 0;
            break;
        }
    }
    cout << sum << endl;

    return 0;
}