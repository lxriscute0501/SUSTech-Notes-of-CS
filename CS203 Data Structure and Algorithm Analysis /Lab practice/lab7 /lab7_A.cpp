#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int a[n + 5];
    for (int i=1; i<=n; i++)
        cin >> a[i];

    bool maxx = true, minn = true;

    for (int i = 1; i <= n/2; i++)
    {
        int l = 2 * i, r = 2 * i + 1;

        if ((l <= n and a[i] > a[l]) or (r <= n and a[i] > a[r])) minn = false;
        if ((r <= n and a[i] < a[r]) or (l <= n and a[i] < a[l])) maxx = false;
        if (!(minn || maxx)) {
            cout << "Neither";
            return 0;
        }
    }

    if (maxx) cout << "Max";
    else if (minn) cout << "Min";
    else cout << "Neither";

    return 0;
}
