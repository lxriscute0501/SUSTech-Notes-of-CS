#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
    int n, k, count, ans=0;
    int a[3000001], maxq[2][3000001], minq[2][3000001];
    cin >> k >> n;
    for (int i=1; i<=n; i++)
        cin >> a[i];
    
    if (n == 1 and k > 0) {
        cout<<0;
        return 0;
    }
    maxq[0][1] = a[1]; minq[0][1] = a[1];
    maxq[1][1] = 1; minq[1][1] = 1;
    count = 1;
    int topmaxq=1, topminq=1, botmaxq=1, botminq=1, q=2;
    while (q <= n)
    {
        if (a[q] > maxq[0][topmaxq]) {
            while (a[q] > maxq[0][topmaxq] and topmaxq >= botmaxq) topmaxq --;
            topmaxq ++;
            maxq[0][topmaxq] = a[q];
            maxq[1][topmaxq] = q;
        } else {
            topmaxq ++;
            maxq[0][topmaxq] = a[q];
            maxq[1][topmaxq] = q;
        }
        if (a[q] < minq[0][topminq]) {
            while (a[q] < minq[0][topminq] and topminq >= botminq) topminq --;
            topminq ++;
            minq[0][topminq] = a[q];
            minq[1][topminq] = q;
        } else {
            topminq ++;
            minq[0][topminq] = a[q];
            minq[1][topminq] = q;
        }
        
        if (maxq[0][botmaxq] - minq[0][botminq] <= k) count ++;
        else {
            bool bo;
            ans = max(ans, count);
            while (maxq[0][botmaxq] - minq[0][botminq] > k)
            {
                if (maxq[1][botmaxq] > minq[1][botminq])
                {
                    botminq ++;
                    bo = false;
                }
                else {
                    botmaxq ++;
                    bo = true;
                }
            }
            if (bo) count = q - maxq[1][botmaxq-1]; else count = q - minq[1][botminq-1];
        }
        q ++;
    }
    cout << max(ans, count);
    return 0;
}
