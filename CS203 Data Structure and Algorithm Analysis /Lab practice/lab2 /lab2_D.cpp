#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, a[1000001], count = 0, x;
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    sort(a + 1, a + 1 + n);
    
    cout << a[n / 3 + 1] << endl;
    for (int i = 1; i <= n / 3 + 1; i++)
        if (a[i] == a[n / 3 + 1]) {
            if (count == 0) x = i;
            count ++;
        }
    
    int k = 1, ans = n / 3 + 1 - count;
        while (k <= n)
        {
            if (k % 3 == 1 and k / 3 + 1 < x) cout << a[k / 3 + 1] << " ";
            else {
                ans ++;
                cout << a[ans] << " ";
            }
            k ++;
        }
    
    return 0;
}
