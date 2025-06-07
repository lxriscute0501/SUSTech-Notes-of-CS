#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

#define ll long long

ll dfs(int u, ll flow, int t, vector<int>& level, vector<int>& ptr, vector<vector<ll>>& cap, vector<vector<int>>& adj) {
    if (u == t or flow == 0) return flow;
    ll total = 0;
    for (; ptr[u] < adj[u].size(); ptr[u]++)
    {
        int v = adj[u][ptr[u]];
        if (level[v] == level[u] + 1 and cap[u][v] > 0) {
            ll x = dfs(v, min(flow, cap[u][v]), t, level, ptr, cap, adj);
            if (x > 0) {
                cap[u][v] -= x;
                cap[v][u] += x;
                total += x;
                flow -= x;
                if (flow == 0) break;
            }
        }
    }
    return total;
}

int main()
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<ll>> cap(n + 1, vector<ll>(n + 1, 0));
    vector<vector<int>> adj(n + 1);

    for (int i = 1; i <= m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        if (u != v) {
            cap[u][v] += w;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    ll ans = 0;
    while (true) {
        vector<int> level(n + 1, -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u])
            {
                if (level[v] == -1 and cap[u][v] > 0) {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }

        if (level[t] == -1) break;

        vector<int> ptr(n + 1, 0);
        while (true) {
            ll pushed = dfs(s, LLONG_MAX, t, level, ptr, cap, adj);
            if (pushed == 0) break;
            ans += pushed;
        }
    }

    cout << ans;
    return 0;
}
