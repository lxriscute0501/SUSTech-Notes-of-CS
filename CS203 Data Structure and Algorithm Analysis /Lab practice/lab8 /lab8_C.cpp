#include <iostream>
#include <vector>
#include <queue>
using namespace std;

inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

int main() 
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t = read();
    
    while (t--) 
    {
        int n = read(), m = read();
        vector<vector<int>> adj(n + 1);
        
        for (int i = 0; i < m; i++) 
        {
            int u = read(), v = read();
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<int> color(n + 1, -1);
        vector<int> set0, set1;
        queue<int> q;
        color[1] = 0;
        q.push(1);
        
        while (!q.empty()) 
        {
            int u = q.front();
            q.pop();
            if (color[u] == 0) set0.push_back(u);
            else set1.push_back(u);

            for (int v : adj[u]) 
            {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                }
            }
        }
        
        if (set0.size() > set1.size()) swap(set0, set1);
        
        cout << set0.size() << '\n';
        for (int i = 0; i < set0.size(); i++) 
            cout << set0[i] << " \n"[i == set0.size() - 1];
    }
    
    return 0;
}