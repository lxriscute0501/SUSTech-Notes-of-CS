#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> g[100001];
int depth[100001], par[100001], sz[100001];
int up[21][100001];

void dfs(int u, int p) {
    depth[u] = depth[p] + 1;
    sz[u] = 1;
    par[u] = p;
    up[0][u] = p;
    for (int v : g[u])
    {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

int father(int u, int k) {
    if (k > depth[u]) return -1;
    
    for (int j = 0; j < 21; j++)
    {
        if (k & (1 << j)) u = up[j][u];
    }
    return u;
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    int diff = depth[u] - depth[v];
    for (int j = 0; j < 21; j++)
    {
        if (diff & (1 << j)) u = up[j][u];
    }
    if (u == v) return u;
    
    for (int j = 20; j >= 0; j--)
    {
        if (up[j][u] != up[j][v]) {
            u = up[j][u];
            v = up[j][v];
        }
    }
    return up[0][u];
}

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    depth[0] = -1;
    dfs(1, 0);

    for (int j = 1; j < 21; j++)
    {
        for (int i = 1; i <= n; i++)
        {
            if (up[j-1][i] > 0) up[j][i] = up[j-1][up[j-1][i]]; else up[j][i] = 0;
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int x, y;
        cin >> x >> y;
        if (x == y) {
            cout << n << '\n';
            continue;
        }

        int lnode = lca(x, y);
        int d1 = depth[x] - depth[lnode], d2 = depth[y] - depth[lnode];
        int L = d1 + d2;

        if (L % 2 == 1) {
            cout << 0 << '\n';
            continue;
        }

        int mid = L / 2;
        int m;
        if (mid <= d1) {
            m = father(x, mid);
        } else {
            m = father(y, d1 + d2 - mid);
        }

        int a = -1, b = -1;
        if (mid > 0) {
            a = father(x, mid - 1);
            b = father(y, mid - 1);
        }

        long long sza = 0, szb = 0;
        if (a != -1) {
            if (par[a] == m) {
                sza = sz[a];
            } else if (par[m] == a) {
                sza = n - sz[m];
            } else {
                if (depth[a] < depth[m]) {
                    if (par[m] == a) sza = n - sz[m];
                } else {
                    if (par[a] == m) sza = sz[a];
                }
            }
        }

        if (b != -1) {
            if (par[b] == m) {
                szb = sz[b];
            } else if (par[m] == b) {
                szb = n - sz[m];
            } else {
                if (depth[b] < depth[m]) {
                    if (par[m] == b) szb = n - sz[m];
                } else {
                    if (par[b] == m) szb = sz[b];
                }
            }
        }

        long long ans = n - sza - szb;
        cout << ans << '\n';
    }

    return 0;
}
