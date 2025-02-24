#include <iostream>
using namespace std;

bool select(long long* c, long long n, long long k, long long mid)
{
    
    long long num = 0;
    long long d[30001];
    
    for (long long i=0; i<n; i++)
        d[i] = c[i];
    
    for (long long i=0; i<n-1; i++)
    {
        if (d[i] >= mid) {
            num += d[i] / mid;
            d[i] -= (d[i] / mid) * mid;
        }
        if (d[i] + d[i+1] >= mid)
        {
            num ++;
            d[i+1] -= mid - d[i];
            d[i] = 0;
        }
    }
    num += d[n-1] / mid;
    if (num >= k) return true; else return false;
     
     
}

int main()
{
    int T;
    cin >> T;
    long long c[30001];
    
    for (int j=1; j<=T; j++)
    {
        long long n, k, l = 1, r = 0, ans = 0;
        cin >> n >> k;
        
        for (long long i=0; i<n; i++)
        {
            cin >> c[i];
            r += c[i];
        }
        
        if (n == 1) {
            cout << c[0] / k * k <<endl;
            continue;
        }
        
        r /= k;
        while (l <= r)
        {
            long long mid = (l + r) / 2;
            if (select(c, n, k, mid)) {
                ans = mid * k;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        cout << ans << endl;
    }
    return 0;
}
