#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t --)
    {
        int a, b;
        cin >> a >> b;
        if (b < a) {
            cout << 0 << endl;
            continue;
        }
        if (a == 0) {
            cout << b + 1 << endl;
            continue;
        }
        b ++;
        int k = 0;
        for (int i = 30; i >= 0; i--)
        {
            int n = a >> i & 1, m = b >> i & 1;
            if (n != m) {
                if (n == 0) k += 1 << i;
            }
            if ((a ^ k) >= b) break;
        }
        cout << k << endl;
    }
    return 0;
}
