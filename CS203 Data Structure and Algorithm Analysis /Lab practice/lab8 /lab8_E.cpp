#include <iostream>
using namespace std;

int dx[9] = {0, 1, 1, 1, 0, 0, -1, -1, -1};
int dy[9] = {0, 0, 1, -1, 1, -1, 0, 1, -1};
int t, n, m, mapp[8][8], v[8][8], s[3][100];
bool bo[8][8];

void dfs(int x, int y, int sum, bool b)
{
    if(y > m) {
        y = 1;
        x ++;
    }
    
    if ((1+b)*n/2 < x)
    {
        int t = 0;
        for (int i=1; i<=m; i++)
            t = t*2 + bo[n/2+b][i];
        if (s[b][t] < sum) s[b][t] = sum;
        return;
    }
    
    if (!v[x][y])
    {
        for (int k=0; k<9; k++)
        {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx >= 1 and nx <= n and ny >= 1 and ny <= m) v[nx][ny] ++;
        }
        
        bo[x][y] = true;
        dfs(x, y+1, sum+mapp[x][y], b);
        for(int k=0; k<9; k++)
        {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx >= 1 and nx <= n and ny >= 1 and ny <= m) v[nx][ny] --;
        }
    }
    bo[x][y] = false;
    dfs(x, y+1, sum, b);
}

int main()
{
    cin >> t;
    while (t --)
    {
        for(int i = 0; i < 64; i++)
        {
            s[0][i] = -1;
            s[1][i] = -1;
        }
        cin >> n >> m;
        for (int i=1; i<=n; i++)
            for (int j=1; j<=m; j++)
            {
                cin >> mapp[i][j];
                v[i][j] = 0;
            }
        
        if(n == 1) {
            long long ans = 0;
            dfs(1, 1, 0, 1);
            
            for(int i = 0; i < 64; i++)
            {
                if (s[1][i] == 0) continue;
                if (s[1][i] > ans) ans = s[1][i];
            }
            cout << ans << endl;
            continue;
        }
        
        dfs(n / 2 + 1, 1, 0, 1);
        dfs(1, 1, 0, 0);
        
        long long ans = 0;
        for(int i=0; i<64; i++)
        {
            if (s[0][i] == -1) continue;
            for(int j=0; j<64; j++)
            {
                bool x = true;
                for(int k=0; k<m; k++)
                {
                    if ((j >> k) & 1)
                    {
                        if ((i >> k) & 1 or (i >> (k + 1)) & 1 or (k and ((i >> (k - 1)) & 1))) x = false;
                    }
                }
                if (s[1][j] == -1 or !x) continue;
                else if (s[0][i] + s[1][j] > ans) ans = s[0][i] + s[1][j];
            }
        }
        cout << ans << endl;
    }
    return 0;
}
