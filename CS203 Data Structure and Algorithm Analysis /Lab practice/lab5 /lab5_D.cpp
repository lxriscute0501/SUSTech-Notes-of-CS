#include <iostream>
#include <string>
using namespace std;

int main()
{
    int n;
    int next[200001];
    cin>>n;
    for (int i=1; i<=n; i++)
    {
        char s[200001];
        scanf("%s", s);
        
        int len=0;
        while (s[len] != '\0') len ++;
        int j=1, k=0;
        
        while (j < len)
        {
            if (s[k] == s[j])
            {
                next[j] = ++k;
                j++;
            } else {
                if (k == 0)
                {
                    next[j] = k;
                    j++;
                } else k = next[k - 1];
            }
        }
        
        int minn = len - next[len-1];
        if (next[len-1] == 0) cout << len << endl;
        else if (len % minn == 0) cout << 0 << endl;
        else cout << minn - (len % minn) << endl;
    }
    
    return 0;
}
