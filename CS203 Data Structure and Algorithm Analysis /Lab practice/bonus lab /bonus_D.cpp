#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, q, t, a[200001], s[200001], dir[200001], r[200001], kk[200001];
    scanf("%d", &t);
    while (t --)
    {
        scanf("%d", &n);
        
        for (int j=1; j<=n; j++)
        {
            scanf("%d", &a[j]);
            r[j] = -1;
        }
        
        scanf("%d", &q);
        for (int j=1; j<=q; j++)
        {
            scanf("%d", &kk[j]);
        }
        
        int k=2, top=1;
        s[1] = a[1]; dir[1] = 1;
        
        while (k <= n)
        {
            if (a[k] <= s[top])
            {
                top ++;
                s[top] = a[k];
                dir[top] = k;
            } else {
                r[dir[top]] = k;
                top --;
                while (top != 0 and a[k] > s[top])
                {
                    r[dir[top]] = k;
                    top --;
                }
                top ++;
                s[top] = a[k];
                dir[top] = k;
            }
            k ++;
        }
        
        for (int j=1; j<=q; j++)
            if (r[kk[j]] != -1) printf("%d\n", r[kk[j]]-kk[j]); else printf("%d\n", r[kk[j]]);
    }
    return 0;
}

