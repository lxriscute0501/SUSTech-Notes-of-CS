#include <iostream>
using namespace std;
int n, m, t;
int g[501][501];

int main()
{
    cin >> t;
    while(t --)
    {
        cin >> n >> m;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                g[i][j] = 0;
        
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            g[u][v] += 1;
        }
        
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++) 
                cout << g[i][j] << " ";
            cout << endl;
        }
    }

    return 0;
}
