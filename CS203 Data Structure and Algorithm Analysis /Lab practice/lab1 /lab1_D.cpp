#include <iostream>
using namespace std;

int main()
{
    int n, m;
    long long t = 1, s = 1, ans = 1;
    cin >> n >> m;
    
    if (m == 1 or n >= 4) {
        cout << 0;
        return 0;
    }
    if (n == 0) {
        cout << 1;
        return 0;
    }
    for (int i = 1; i <= n; i++)
        t *= i;
    for (int i = 1; i <= t; i++)
        s *= i;
    for (int i = 1; i <= s; i++)
    {
        ans *= i;
        ans %= m;
    }
    cout << ans;
    return 0;
}
