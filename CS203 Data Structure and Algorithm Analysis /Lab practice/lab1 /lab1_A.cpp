//binary search model
#include <iostream>
using namespace std;

int main()
{
    int n, t, x, mid, l, r;
    int a[100001];
    bool b;
    
    cin >> n;
    for (int i=1; i<=n; i++)
        cin >> a[i];
    
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        l = 1; r = n; b = false;
        cin >> x;
        while (l <= r)
        {
            mid = (l + r) / 2;
            if (x == a[mid]) {
                b = true;
                break;
            } else if (x > a[mid]) l = mid + 1; else r = mid - 1;
        }
        if (b) cout << "YES" << endl; else cout << "NO" << endl;
    }
    return 0;
}
