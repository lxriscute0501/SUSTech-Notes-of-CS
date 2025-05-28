#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> cookies;

    for (int i = 1; i <= m; i++)
    {
        int op;
        cin >> op;
        if (op == 0) {
            int c, v;
            cin >> c >> v;
            cookies.push_back({0, c, v});
        } else if (op == 1) {
            int c, v;
            cin >> c >> v;
            cookies.push_back({1, c, v});
        } else if (op == 2) {
            int x, c, v;
            cin >> x >> c >> v;
            cookies.push_back({2, x, c, v});
        }
    }

    int dp[n+1];
    for (int i=1; i<=n; i++)
        dp[i] = INT_MIN;
    dp[0] = 0;

    for (auto& cookie : cookies)
    {
        int op = cookie[0];
        if (op == 0) {
            int c = cookie[1], v = cookie[2];
            for (int i = c; i <= n; i++)
            {
                if (dp[i - c] != INT_MIN) {
                    dp[i] = max(dp[i], dp[i - c] + v);
                }
            }
        } else if (op == 1) {
            int c = cookie[1], v = cookie[2];
            for (int i = n; i >= c; i--)
            {
                if (dp[i - c] != INT_MIN) {
                    dp[i] = max(dp[i], dp[i - c] + v);
                }
            }
        } else if (op == 2) {
            int x = cookie[1], c = cookie[2], v = cookie[3];
            int k = 1;
            
            while (x > 0)
            {
                int curr = min(k, x);
                int cost = curr * c, value = curr * v;
                
                for (int i = n; i >= cost; i--)
                {
                    if (dp[i - cost] != INT_MIN) {
                        dp[i] = max(dp[i], dp[i - cost] + value);
                    }
                }
                
                x -= curr;
                k *= 2;
            }
        }
    }

    cout << (dp[n] == INT_MIN ? 0 : dp[n]);
    return 0;
}
