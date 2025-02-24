#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    char ss[1000001];
    char ch[27];
    int next[1000001];
    
    for (int i=0; i<26; i++)
        cin >> ch[i];
    
    int len = 0;
    scanf("%s", ss);
    while (ss[len] != '\0') len ++;
    
    for (int i=0; i<len; i++)
    {
        ss[len+i] = ch[int(ss[i]) - int('a')];
    }
   
    len *= 2;
    int n = len, j, k;
    
            j = 1; k = 0;
            
            while (j < len)
            {
                if (ss[k] == ss[j])
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
            while (next[len-1] > n/4) len = next[len-1];
    cout<<n/2 - next[len-1];

    return 0;
}
