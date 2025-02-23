#include <iostream>
using namespace std;

int main()
{
    int t, a[100001], b[100001];
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        int n, m, k1 = 1, k2 = 1;
        cin >> n >> m;
        for (int j=1; j<=n; j++)
            cin >> a[j];
        for (int j=1; j<=m; j++)
            cin >> b[j];
        
        while (k1 <= n or k2 <= m)
        {
            if (k1 > n) {
                for (int j=k2; j<=m; j++)
                    cout << b[j] << " ";
                break;
            }
            if (k2 > m) {
                for (int j=k1; j<=n; j++)
                    cout << a[j] << " ";
                break;
            }
            if (a[k1] <= b[k2]) {
                cout << a[k1] << " ";
                k1 ++;
            } else {
                cout << b[k2] << " ";
                k2 ++;
            }
        }
        cout << endl;
        
    }
    return 0;
}
