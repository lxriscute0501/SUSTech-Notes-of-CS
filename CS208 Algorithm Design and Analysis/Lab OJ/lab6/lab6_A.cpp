#include <iostream>
using namespace std;

long long cnt = 0;

void merge(int* a, int left, int right, int mid) {
    int* b = new int [right - left + 1];
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid and j <= right)
    {
        if (a[i] <= a[j]) {
            b[k] = a[i];
            k ++; i ++;
        }
        else {
            cnt += mid - i + 1;
            b[k] = a[j];
            k ++; j ++;
        }
    }
    
    while (i <= mid) {
        b[k] = a[i];
        k ++; i ++;
    }
    
    while (j <= right) {
        b[k] = a[j];
        k ++; j ++;
    }
    
    k = 0;
    for (int i = left; i <= right; i++)
    {
        a[i] = b[k];
        k ++;
    }
    
    delete[] b;
}


void mergesort(int* a, int left, int right) {
    if (left < right) {
        int mid = (right - left) / 2 + left;
        mergesort(a, left, mid);
        mergesort(a, mid + 1, right);
        merge(a, left, right, mid);
    }
}

int main()
{
    int n;
    cin >> n;
    int a[n];
    for (int i = 0; i < n; i ++)
        cin >> a[i];
    
    mergesort(a, 0, n - 1);
    
    cout << cnt;
    return 0;
}
