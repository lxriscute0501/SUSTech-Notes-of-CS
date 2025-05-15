#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, l;
    int ma, mb, xa, xb, ca, cb;
    long long opt[5005][5005];
    bool b[1000005];
    cin >> n >> l;
    cin >> ma >> xa >> ca;
    cin >> mb >> xb >> cb;
    
    b[0] = true; b[l] = true;
    for (int i = 1; i < l; i++)
        b[i] = false;
    
    for (int i = 1; i <= n; i++)
    {
        int x;
        cin >> x;
        b[x] = true;
    }
    
    for (int x = 0; x <= xa; x++)
        for (int y = 0; y <= xb; y++)
            opt[x][y] = 1e16;
    opt[0][0] = 0;
    
    for (int x = 0; x <= xa; x++)
        for (int y = 0; y <= xb; y++)
        {
            int loc = x * ma + y * mb;
            if (loc > l or !b[loc]) continue;
            if (x > 0 and b[loc - ma]) opt[x][y] = min(opt[x][y], opt[x - 1][y] + ca);
            if (y > 0 and b[loc - mb]) opt[x][y] = min(opt[x][y], opt[x][y - 1] + cb);
        }
    
    long long ans = 1e16;
    for (int i = 0; i <= xa; i++)
    {
        if (l < i * ma or (l - i * ma) % mb != 0) continue;
        ans = min(ans, opt[i][(l - i * ma) / mb]);
    }
    if (ans == 1e16) cout << -1; else cout << ans;
    return 0;
}
