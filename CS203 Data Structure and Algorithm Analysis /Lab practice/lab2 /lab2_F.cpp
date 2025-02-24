#include <iostream>
#include <algorithm>
using namespace std;

void merge(int* a, int* col, int low, int mid, int hight)
{
    int* b = new int [hight - low + 2];
    int* c = new int [hight - low + 2];
    int i = low, j = mid + 1, k = 1;
    while (i <= mid and j <= hight)
    {
        if (a[i] <= a[j]) {
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
    
    delete[] b;
    delete[] c;
}

void mergesort(int* a, int* col, int low, int hight)
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
    int t, red1[100001], blue1[100001], red2[100001], blue2[100001];
    cin >> t;
    for (int i = 1; i <= t; i++)
    {
        int n, x, y, k1 = 0, k2 = 0, ans = 0;
        cin >> n;
        for (int j = 1; j <= n; j++)
        {
            cin>> x >> y;
            if (x <= y) {
                k1 ++;
                blue1[k1] = x;
                red1[k1] = y;
            } else {
                k2 ++;
                blue2[k2] = x;
                red2[k2] = y;
            }
        }
        if (k1 >= 2) mergesort(blue1,red1,1,k1);
        if (k2 >= 2) mergesort(red2,blue2,1,k2);
        
        for (int j = 1; j < k1; j++)
        {
            ans += min(red1[j], blue1[j+1]);
            if (red1[j] > blue1[j+1]) {
                red1[j+1] += red1[j] - blue1[j+1];
                blue1[j+1] = blue1[j];
            } else if (red1[j] < blue1[j+1]) {
                blue1[j+1] += blue1[j] - red1[j];
            }
            else blue1[j+1] = blue1[j];
        }
        
        for (int j = k2; j > 1; j--)
        {
            ans += min(red2[j], blue2[j-1]);
            if (red2[j] > blue2[j-1]) {
                red2[j-1] += red2[j] - blue2[j-1];
                blue2[j-1] = blue2[j];
            } else if (red2[j] < blue2[j-1]) {
                blue2[j-1] += blue2[j] - red2[j];
            }
            else blue2[j-1] = blue2[j];
        }
        if (k1 >= 1 and k2 >= 1) ans += max(min(red1[k1], blue2[1]), min(red2[1], blue1[k1]));
        cout<< ans << endl;
    }
    
    return 0;
}
