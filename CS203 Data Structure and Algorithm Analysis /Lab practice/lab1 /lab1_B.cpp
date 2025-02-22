#include <iostream>
using namespace std;

int main()
{
    int n;
    long long t;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> t;
        long long x = t * (t + 1);
        x /= 2;
        x %= 1000000007;
        long long y = (x * x) % 1000000007; //立方和公式，注意开 long long 防止数据溢出
        cout << y << endl;
    }
    
    return 0;
}
