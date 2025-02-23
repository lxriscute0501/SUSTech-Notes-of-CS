#include <iostream>
using namespace std;

int main()
{
    int a[100001], b[100001];
    int n, t, l, r;
    
    cin >> n >> t;
    for (int i=1; i<=n; i++)
        cin >> a[i];
    for (int i=1; i<=n; i++)
        cin >> b[i];
    
    for (int i=1; i<=t; i++)
    {
        cin >> l >> r;
        
        int la = l, lb = l, count = r - l + 1;
        int mida = l + count / 2 - 1, midb = l + count / 2 - 1;
        
        if (r == l) {
            cout << min(a[l], b[l]) << endl;
            continue;
        }
        if (a[r] <= b[l]) {
            cout << a[r] << endl;
            continue;
        }
        if (a[l] >= b[r]) {
            cout << b[r] << endl;
            continue;
        }
        while (count > 1)
        {
            if (a[mida] <= b[midb]) {
                count -= mida - la + 1;
                la = mida + 1;
                    mida = max(la, la + count / 2 - 1);
                    midb = max(lb, lb + count / 2 - 1);
                continue;
            }
            if (a[mida] > b[midb]) {
                count -= midb - lb + 1;
                lb = midb + 1;
                mida = max(la, la + count / 2 - 1);
                midb = max(lb, lb + count / 2 - 1);
                continue;
            }
        }
        if (count == 1) cout << min(a[mida], b[midb]) << endl;
    }
    
    return 0;
}

