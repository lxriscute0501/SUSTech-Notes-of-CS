#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int d[n+1], t[n+1], w[n+1];
    int maxx = 0, total = 0, sumt = 0;
    for (int i = 1; i <= n; i++)
    {
        cin >> d[i] >> t[i] >> w[i];
        sumt += t[i];
        total += w[i];
    }

    int dp[n+1][n+1];
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            dp[i][j] = 0;
    
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= sumt; j++)
        {
            dp[i][j] = dp[i-1][j];
            if (j > 0 and j <= d[i]) dp[i][j] = max(dp[i][j], dp[i-1][j-t[i]] + w[i]);
        }
    }

    for (int j = 0; j <= sumt; j++)
        maxx = max(maxx, dp[n][j]);

    cout << total - maxx;
    return 0;
}
