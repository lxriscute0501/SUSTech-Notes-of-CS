#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, m, c, t;
    cin >> n >> m >> c >> t;
    
    int p[n + 1], q[m + 1];
    for (int i = 1; i <= n; i++)
        cin >> p[i];
    for (int i = 1; i <= m; i++)
        cin >> q[i];
    
    sort(p + 1, p + 1 + n);
    sort(q + 1, q + 1 + m);
    
    int k = 1, ans = 0;
    for (int i = 1; i <= m; i++)
    {
        int cnt = 0;
        while (k <= n and cnt < c and p[k] <= q[i] + t)
        {
            if (p[k] >= q[i] - t) {
                cnt ++;
                ans ++;
            }
            k ++;
        }
    }
    
    cout << ans;
    return 0;
}
