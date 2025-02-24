#include <iostream>
using namespace std;

int main()
{
    int t;
    cin>>t;
    for (int i=1; i<=t; i++)
    {
        int n, a[300001], st[300001], m=1, k=1, top=0;
        bool b[300001];
        
        cin>>n;
        for (int j=1; j<=n; j++)
        {
            cin>>a[j];
            b[a[j]] = false;
        }
        
        while (k <= n)
        {
            if (a[k] != m) {
                top ++;
                st[top] = a[k];
                b[a[k]] = true;
            } else {
                b[a[k]] = true;
                cout << a[k] << " ";
                while (b[m]) {
                    m ++;
                }
                while (top > 0 and st[top] < m) {
                    cout << st[top] << " ";
                    top --;
                }
            }
            k ++;
        }
        
        cout<<endl;
    }
    return 0;
}
