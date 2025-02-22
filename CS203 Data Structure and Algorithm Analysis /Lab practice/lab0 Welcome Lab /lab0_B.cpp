//考虑到数据大小与数量，用Hash表
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
        a[s] += 1; //Hash，可换成bool数组
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
