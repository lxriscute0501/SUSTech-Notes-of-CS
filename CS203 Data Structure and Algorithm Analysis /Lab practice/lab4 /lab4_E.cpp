#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, t, a[50001], s[50001], dir[50001], r[50001], l[50001];
    scanf("%d", &t);
    for (int i=1; i<=t; i++)
    {
        scanf("%d", &n);
        
        for (int j=1; j<=n; j++)
        {
            scanf("%d", &a[j]);
            r[j] = j+1;
            l[j] = j-1;
        }
        r[n] = 0; l[1] = 0;
        
        int k=2, top=1;
        s[1] = a[1]; dir[1] = 1;
        
        while (k <= n)
        {
            if (a[k] < s[top]) {
                top ++;
                s[top] = a[k];
                dir[top] = k;
                l[k] = 0;
            } else {
                r[dir[top]] = 0;
                int maxx = s[top], kk = dir[top];
                top --;
                while (top != 0 and a[k] > s[top])
                {
                    if (maxx < s[top]) { maxx = s[top]; kk = dir[top]; }
                    r[dir[top]] = dir[top+1];
                    top --;
                }
                l[k] = kk;
                r[dir[top]] = k;
                top ++;
                s[top] = a[k];
                dir[top] = k;
            }
            k ++;
        }
        
        printf("Case %d:\n", i);
        for (int j=1; j<=n; j++)
            printf("%d %d\n", l[j], r[j]);
    }
    return 0;
}

