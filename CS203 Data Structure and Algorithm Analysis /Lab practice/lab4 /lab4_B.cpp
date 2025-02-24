#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

int main()
{
    string s;
    int a[100001], top = 1, ans = 0;
    cin>>s;
    a[1] = 1;
    for (int i = 1; i < s.size(); i++)
    {
        if (s[i] == '(') {
            top ++;
            if (top > 1) a[top] = (a[top - 1] * 2) % 514329; else a[top] = 1;
        } else {
            if (s[i - 1] == '(')
            {
                ans += a[top];
                ans %= 514329;
            }
            top --;
        }
    }
    cout << ans;
  
    return 0;
}
