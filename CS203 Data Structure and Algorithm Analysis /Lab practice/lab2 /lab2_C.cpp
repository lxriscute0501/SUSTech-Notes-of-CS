#include <iostream>
using namespace std;

int main()
{
    int a[3001];
    int n, m, t, l, r, k = 1;
    long long count = 0;
    
    cin >> n >> m;
    for (int i=1; i<=n; i++)
        cin >> a[i];
    for (int i=1; i<=n-1; i++)
        for (int j=i; j<=n; j++)
            if (a[i] > a[j]) {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
    
    while (k <= n - 2)
    {
        int c = m - a[k], ll, rr;
        l = k + 1; r = n;
        
        while (l <= r)
        {
            if (a[l] + a[r] > c) r --;
            else if (a[l] + a[r] < c) l ++;
            else {
                ll = l; rr = r;
                while (a[ll + 1] == a[ll] and ll <= r - 1) ll ++;
                if (ll == r) {
                    count += (r - l + 1) * (r - l) / 2;
                    break;
                }
                while (a[rr - 1] == a[rr]) rr --;
                count += (ll - l + 1) * (r - rr + 1);
                l = ll + 1; r = rr - 1;
            }
        }
        k ++;
    }
    cout << count;
    return 0;
}
