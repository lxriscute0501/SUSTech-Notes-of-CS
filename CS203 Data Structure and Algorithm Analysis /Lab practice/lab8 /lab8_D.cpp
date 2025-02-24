#include <iostream>
#include <vector>
using namespace std;

vector<int> g[150001];
int a[150001], s[150001], sum;
bool fa[150001], visited[150001];
int start[150001], endt[150001];

inline int inget()
{
    int i = 0;
    char nagative = 0;
    char ch = '\0';
    
    while((ch = getchar()) < 48 || ch > 57)
        if (ch == '-') nagative = 1;
    
    while(ch > 47 and ch < 58)
    {
        i = (i << 1) + (i << 3) + (ch & 0xCF);
        ch = getchar();
    }
    if (nagative) i = -i;
    return i;
}


void dfs(int u)
{
    a[u] = ++sum;
    start[u] = a[u];
    for (int i = 0; i < g[u].size(); i++)
    {
        int v = g[u][i];
        if (visited[v]) continue;
        visited[v] = true;
        dfs(v);
        s[u] += s[v];
    }
    endt[u] = a[u] + s[u] - 1;
}

int main()
{
    int t, n, m, x, y, root;
    t = inget();
    for (int j = 1; j <= t; j++)
    {
        n = inget(); m = inget();
        sum = 0;
        for (int i = 1; i <= n; i++)
        {
            g[i].clear();
            fa[i] = false;
            visited[i] = false;
            s[i] = 1;
        }
        for (int i = 0; i < n - 1; i++)
        {
            x = inget(); y = inget();
            g[y].push_back(x);
            g[x].push_back(y);
            fa[x] = true;
        }

        root = 0;
        for (int i = 1; i <= n; i++)
            if (!fa[i]) {
                root = i;
                break;
            }

        visited[root] = true;
        dfs(root);

        for (int i = 0; i < m; i++)
        {
            x = inget(); y = inget();
            if (start[x] >= start[y] and start[x] <= endt[y]) printf("Yes\n"); else printf("No\n");
        }
    }
    return 0;
}
