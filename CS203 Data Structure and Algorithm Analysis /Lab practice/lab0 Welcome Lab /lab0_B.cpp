#include <iostream>
using namespace std;

int main()
{
    int n, t, s;
    int a[100001];
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> s;
        a[s] += 1;
    }
    cin >> t;
    for (int i = 1; i <= t; i++)
    {
        int num;
        cin >> num;
        if (a[num] > 0) cout<<"yes\n"; else cout<<"no\n";
    }
    return 0;
}
