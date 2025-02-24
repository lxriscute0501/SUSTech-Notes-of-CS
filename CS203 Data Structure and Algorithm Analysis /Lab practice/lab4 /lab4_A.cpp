#include <iostream>
using namespace std;

int main()
{
    int t;
    char s[30001];
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        int n, top = 0;
        cin >> n;
        for (int j=1; j<=n; j++)
        {
            char ch;
            cin >> ch;
            if (top >= 0 and (ch == '(' or ch == '[' or ch == '{')) {
                top ++;
                s[top] = ch;
            }
            if (top >= 0 and (ch == ')' or ch == ']' or ch == '}')) {
                if (s[top] == '(' and ch == ')') top --;
                else if (s[top] == '[' and ch == ']') top --;
                else if (s[top] == '{' and ch == '}') top --;
                else top = -1;
            }
        }
        if (top == 0) cout<<"YES"<<endl; else cout<<"NO"<<endl;
    }
  
    return 0;
}
