#include <iostream>
using namespace std;

int main()
{
    int t;
    int a[200001], dp[200001];
    cin >> t;
    while (t --)
    {
        int n, last, ans = -100001;
        cin >> n;
        cin >> last;
        for (int i = 1; i < n; i++)
        {
            int x;
            cin >> x;
            a[i] = last - x;
            last = x;
        }

        //最大子序和
        dp[1] = a[1];
        for (int i = 2; i < n; i++)
        {
            if (dp[i - 1] <= 0) dp[i] = a[i];
            else dp[i] = dp[i - 1] + a[i];
        }
        
        for (int i = 1; i < n; i++)
            ans = max(ans, dp[i]);
        cout << ans << endl;
    }
    
    return 0;
}
