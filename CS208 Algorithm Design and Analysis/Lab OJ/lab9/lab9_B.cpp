#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int n, m;
    long long c[4001], w[4001], cnt[4001];
    long long dp[40001], choice[40001];
    cin >> n >> m;
    for (int i = 1; i <= m; i++)
        cin >> c[i] >> w[i];

    for (int i = 0; i <= n; i++)
    {
        dp[i] = 0;
        choice[i] = -1;
    }
    for (int i = 1; i <= m; i++)
        cnt[i] = 0;

    for (int i = 1; i <= m; i++)
    {
        for (long long j = c[i]; j <= n; j++)
        {
            if (dp[j - c[i]] + w[i] > dp[j]) {
                dp[j] = dp[j - c[i]] + w[i];
                choice[j] = i;
            }
        }
    }

    int x = n;
    while (x > 0 and choice[x] != -1)
    {
        long long idx = choice[x];
        cnt[idx] ++;
        x -= c[idx];
    }

    cout << dp[n] << endl;
    for (int i = 1; i <= m; i++)
        cout << cnt[i] << endl;

    return 0;
}
