#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

vector<vector<pair<int, int>>> g;
vector<int> sub;
unordered_set<int> flag;
vector<bool> v;
int n, k;
bool found = false;

void gett(int u, int father, int cur, vector<int>& dist) {
    dist.push_back(cur);
    for (auto& [x, y] : g[u])
    {
        if (!v[x] and x != father) gett(x, u, cur + y, dist);
    }
}

void calc(int u) {
    flag.clear();
    flag.insert(0);
    
    for (auto& [x, y] : g[u])
    {
        if (!v[x]) {
            vector<int> dist;
            gett(x, u, y, dist);
            
            for (int d : dist)
            {
                if (d == k or flag.count(k - d)) {
                    found = true;
                    return;
                }
            }
            
            for (int d : dist)
                flag.insert(d);
        }
    }
}

void subsize(int u, int father) {
    sub[u] = 1;
    for (auto& [x, y] : g[u])
    {
        if (!v[x] and x != father) {
            subsize(x, u);
            sub[u] += sub[x];
        }
    }
}

int find(int u, int father, int sum) {
    for (auto& [x, y] : g[u]) {
        if (!v[x] and x != father and sub[x] > sum / 2) {
            return find(x, u, sum);
        }
    }
    return u;
}

void divide(int u) {
    subsize(u, -1);
    int centroid = find(u, -1, sub[u]);
    v[centroid] = true;
    calc(centroid);
    
    if (found) return;
    
    for (auto& [x, y] : g[centroid])
    {
        if (!v[x]) {
            divide(x);
            if (found) return;
        }
    }
}

int main()
{
    cin >> n >> k;
    g.resize(n + 1);
    sub.resize(n + 1);
    v.resize(n + 1, false);

    for (int i = 1; i < n; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    divide(1);
    cout << (found ? "Yes" : "No") << endl;

    return 0;
}
