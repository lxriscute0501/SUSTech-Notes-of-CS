#include <iostream>
using namespace std;

long long n, a[100005];
long long cost = 0;

//归并排序模版
void merge(long long* a, long long low, long long mid, long long hight)
{
    long long* b = new long long[hight - low + 2];
    long long i = low, j = mid + 1, k = 1;
    while (i <= mid and j <= hight)
    {
        if (a[i] <= a[j]) {
            b[k] = a[i];
            k ++; i ++;
        }
        else {
            cost += (mid - i + 1) * a[j];
            b[k] = a[j];
            k ++; j ++;
        }
    }
    while (i <= mid)
    {
        b[k] = a[i];
        k ++; i ++;
    }
    while (j <= hight)
    {
        b[k] = a[j];
        k ++; j ++;
    }
    
    k = 0;
    for (long long i = low; i <= hight; i++)
    {
        k ++;
        a[i] = b[k];
    }
    delete[] b;
}

void mergesort(long long* a, long long low, long long hight)
{
    if (low < hight)
    {
        long long mid = (low + hight) / 2;
        mergesort(a, low, mid);
        mergesort(a, mid + 1, hight);
        merge(a, low, mid, hight);
    }
}

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    
    mergesort(a, 1, n);
    cout<<cost<<endl;
    return 0;
}
