#include <iostream>
#include <algorithm>
using namespace std;

int n, m, k=0, fa[100010];
long long ans;

struct edge {
    int u, v, w;
    
    bool operator < (const edge &rhs) const {
        return w > rhs.w;
    }
} edges[100010];

int find(int x)
{
    if (x == fa[x]) return x;
    return fa[x] = find(fa[x]);
}


int main()
{
    cin >> n >> m;
    int map[n+5][m+5];
    
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            cin >> map[i][j];

    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++) {
            if(j != m) {
                edges[k].u = (i-1)*m+j;
                edges[k].v = edges[k].u+1;
                edges[k++].w = map[i][j] * map[i][j+1];
            }
            if(i != n) {
                edges[k].u = (i-1)*m+j;
                edges[k].v = i*m+j;
                edges[k++].w = map[i][j] * map[i+1][j];
            }
        }
    
    sort(edges, edges+k);
    for (int i=0; i<=n*m; i++)
        fa[i] = i;
    int cnt = 0;
    for(int i=0; i<k; i++)
    {
        int u = edges[i].u, v = edges[i].v;
        int fu = find(u), fv = find(v);
        if (fu != fv) {
            cnt ++;
            ans += edges[i].w;
            fa[fu] = fv;
        }
        if(cnt == n*m-1) break;
    }
    cout << ans;

    return 0;
}
