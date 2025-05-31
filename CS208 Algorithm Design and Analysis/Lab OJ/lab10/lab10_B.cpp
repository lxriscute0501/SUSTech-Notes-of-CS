#include <iostream>
using namespace std;

const int MOD = 99824353;

int main()
{
    int n, m;
    cin >> n >> m;

    if (m == 0) {
        cout << 1 << endl;
        return 0;
    }

    long long dp[505][505];
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            dp[i][j] = 0;
    
    if (m > n) m = n;
    
    dp[0][0] = dp[1][1] = 1;
    for (int i = 1; i <= m; i++)
    {
        dp[i][0] = 1;
        if (i + 1 <= m) dp[1][i+1] = 1;
    }
        
    for (int i = 1; i <= n; i++)
    {
        int l = (i-m < 1) ? 1 : i-m;
        int r = (i+m > n) ? n : i+m;
        
        dp[i][l] = (dp[i-1][l-1] + dp[i-1][l]) % MOD;
        for (int j = l + 1; j <= r; j++)
        {
            dp[i][j] = (dp[i-1][j] + dp[i][j-1]) % MOD;
        }
    }

    cout << dp[n][n] % MOD << endl;
    return 0;
}
