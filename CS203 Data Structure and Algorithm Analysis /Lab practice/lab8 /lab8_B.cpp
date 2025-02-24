#include<iostream>
#include<vector>
using namespace std;

int n, m, t, s, a[100001];
bool vi[100001];
vector<int> g[100001];

int main()
{
    cin >> t;
    while(t--)
    {
        cin >> n >> m >> s;
        for(int i = 1; i <= n; i++)
        {
            a[i] = -1;
            vi[i] = false;
            g[i].clear();
        }

        for(int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        
        vector<int> q;
        q.push_back(s);
        vi[s] = true;
        a[s] = 0;

        int front = 0;
        while(front < q.size())
        {
            int u = q[front];
            front++;
            for(int i = 0; i < g[u].size(); i++)
            {
                int v = g[u][i];
                if(vi[v]) continue;
                a[v] = a[u] + 1;
                vi[v] = true;
                q.push_back(v);
            }
        }

        for(int i = 1; i <= n; i++)
            cout << a[i] << " ";
        cout << endl;
    }

    return 0;
}
