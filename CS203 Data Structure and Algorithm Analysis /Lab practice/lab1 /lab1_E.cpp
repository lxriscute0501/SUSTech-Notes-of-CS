#include <iostream>
#include <algorithm>
using namespace std;

int l, n, m, le, ri, mid, ans;
int a[500001];

bool panduan()
{
    int count = 1, t = 0, k = 1;
    while (k <= n + 1)
    {
        if (a[k] - t > mid) {
            count ++;
            t = a[k - 1];
            if (count > m) return false;
        }
        k ++;
    }
    return true;
}

int main()
{
    
    while (std::cin >> l >> n >> m)
    {
        if (n == 0 or m == 1) {
            cout << l << endl;
            continue;
        }
        
        for (int i = 1; i <= n; i++)
            cin >> a[i];
        
        sort(a + 1, a + 1 + n);
        a[0] = 0; a[n + 1] = l;
        le = 1;
        
        for (int i = 1; i <= n + 1; i++)
            if (le < a[i] - a[i - 1]) le = a[i] - a[i - 1];
        
        //二分答案
        ri = l; ans = l;
        while (le <= ri)
        {
            mid = (le + ri) / 2;
            if (panduan()) {
                ans = mid;
                ri = mid - 1;
            } else le = mid + 1;
        }
        cout << ans << endl;
    }
    return 0;
}
