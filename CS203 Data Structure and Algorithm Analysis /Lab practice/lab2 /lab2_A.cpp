#include <iostream>
using namespace std;

int main()
{
    int t, a[101];
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        int n, s;
        cin >> n;
        for (int j=1; j<=n; j++)
            cin >> a[j];
        for (int j=1; j<=n-1; j++)
            for (int k=j; k<=n; k++)
                if (a[j] < a[k]) {
                    s = a[j];
                    a[j] = a[k];
                    a[k] = s;
                }
        if (a[2] != a[3] and a[4] != a[3]) cout << a[3] << endl; else cout << "wa" << endl;
    }
    
    return 0;
}
