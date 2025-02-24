#include <iostream>
#include <vector>
using namespace std;

vector<int> g[50001], gg[50001], rg[50001];
int color[50001], vi[50001], low[50001], st[50001], dfn[50001], cnt[50001];
int deep, top, sum, res = 0;
int n, m, s;

void tarjan(int v)
{
    vi[v] = 1;
    st[++top] = v;
    dfn[v] = ++deep;
    low[v] = deep;
    
    for (int i=0; i<g[v].size(); i++)
    {
        int id = g[v][i];
        if (!dfn[id]) {
            tarjan(id);
            if (low[id] < low[v]) low[v] = low[id];
        }
        else {
            if (vi[id] and low[id] < low[v]) low[v] = low[id];
        }
    }

    if (low[v] == dfn[v]) {
        color[v] = ++sum;
        vi[v] = 0;
        while (st[top] != v)
        {
            color[st[top]] = sum;
            vi[st[top--]] = 0;
        }
        top--;
    }
}

int main()
{
    cin>>n>>m>>s;
    for(int i=1; i<=m; i++)
        {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
        }

        for (int i=1; i<=n; i++)
            if (!dfn[i]) tarjan(i);
        
        for (int u=1; u<=n; u++)
        {
            for (int i = 0; i < g[u].size(); i++)
            {
                int v = g[u][i];
                if (color[u] != color[v]) {
                    gg[color[u]].push_back(color[v]);
                    rg[color[v]].push_back(color[u]);
                }
            }
        }
        int ans = 0;
        for (int i=1; i<=sum; i++) {
            if (rg[i].size() == 0) ans++;
        }
    if (rg[color[s]].size() == 0) cout << ans - 1; else cout << ans;
    return 0;
}
