#include <iostream>
using namespace std;

int main()
{
    int t;
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        long long n, k, ans = 0, count = 1;
        cin >> n >> k;
        while (true)
        {
            if (ans + count <= n) {
                if (ans + count == n) {
                    cout << count << endl;
                    break;
                }
                ans += count;
                count *= k;
            } else {
                ans += (n - ans - 1) / k + 1;
                ans -= count / k;
                cout << n - ans << endl;
                break;
            }
        }
    }
    
    return 0;
}
