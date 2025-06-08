#include <iostream>
#include <algorithm>
using namespace std;

#define ll long long

void merge(ll* a, ll* col, int low, int mid, int hight)
{
    ll b[hight - low + 2], c[hight - low + 2];
    int i = low, j = mid + 1, k = 1;
    while (i <= mid and j <= hight)
    {
        if (a[i] >= a[j]) {
            b[k] = a[i];
            c[k] = col[i];
            k ++; i ++;
        }
        else {
            b[k] = a[j];
            c[k] = col[j];
            k ++; j ++;
        }
    }
    
    while (i <= mid)
    {
        b[k] = a[i];
        c[k] = col[i];
        k ++; i ++;
    }
    while (j <= hight)
    {
        b[k] = a[j];
        c[k] = col[j];
        k ++; j ++;
    }
    
    k = 0;
    for (int i = low; i <= hight; i++)
    {
        k ++;
        a[i] = b[k];
        col[i] = c[k];
    }
}

void mergesort(ll* a, ll* col, int low, int hight)
{
    if (low < hight)
    {
        int mid = (low + hight) / 2;
        mergesort(a, col, low, mid);
        mergesort(a, col, mid + 1, hight);
        merge(a, col, low, mid, hight);
    }
}

int main()
{
    ll n, k;
    cin >> n >> k;
    ll x[n+1], m[n+1];
    for (int i = 1; i <= n; i++)
        cin >> x[i];
    for (int i = 1; i <= n; i++)
        cin >> m[i];

    ll lx[n+1], lm[n+1], rx[n+1], rm[n+1];
    int numl = 0, numr = 0;
    for (int i = 1; i <= n; i++)
    {
        if (x[i] < 0) {
            numl++;
            lx[numl] = -x[i];
            lm[numl] = m[i];
        } else {
            numr++;
            rx[numr] = x[i];
            rm[numr] = m[i];
        }
    }

    mergesort(lx, lm, 1, numl);
    mergesort(rx, rm, 1, numr);
    
    ll ans = 0, cap = 0;
    for (int i = 1; i <= numl; i++)
    {
        ll absx = lx[i], r = lm[i];
        if (cap > 0) {
            if (r >= cap) {
                r -= cap;
                cap = 0;
            } else {
                cap -= r;
                r = 0;
            }
        }
        if (r == 0) continue;

        ans += 2 * (r / k) * absx;
        if (r % k > 0) {
            ans += 2 * absx;
            cap = k - r % k;
        } else cap = 0;
    }

    cap = 0;
    for (int i = 1; i <= numr; i++)
    {
        ll absx = rx[i], r = rm[i];
        if (cap > 0) {
            if (r >= cap) {
                r -= cap;
                cap = 0;
            } else {
                cap -= r;
                r = 0;
            }
        }
        if (r == 0) continue;

        ans += 2 * (r / k) * absx;
        if (r % k > 0) {
            ans += 2 * absx;
            cap = k - r % k;
        } else cap = 0;
    }

    ll maxx;
    if (numl >= 1 and numr >= 1) maxx = max(lx[1], rx[1]);
    else if (numl < 1) maxx = rx[1]; else maxx = lx[1];
    ans -= maxx;
    cout << ans;
    return 0;
}
