#include <iostream>
using namespace std;

int main()
{
    int n, q, x, y, l, r, mid, s;
    int a[100001];
    
    cin >> n >> q;
    for (int i=1; i<=n; i++)
        cin >> a[i];
    for (int i=1; i<=q; i++)
    {
        cin >> x >> y;
        if (x >= y) {
            cout << "NO" << endl;
            continue;
        }
        
        //找左界
        l = 1; r = n; mid = (l + r) / 2; s = 0;
        while (l <= r)
        {
            if (x >= a[mid]) l = mid + 1; else r = mid - 1;
            mid = (l + r) / 2;
        }
        if (x >= a[mid]) s = mid + 1; else s = mid;
        
        //找右界
        l = mid; r = n; mid = (l + r) / 2;
        while (l <= r)
        {
            if (y > a[mid]) l = mid + 1; else r = mid - 1;
            mid = (l + r) / 2;
        }
        if (y <= a[mid]) mid --;
        
        if (mid - s + 1 > 0) cout << "YES " << mid - s + 1 << endl; else cout << "NO" << endl;
    }

    return 0;
}
