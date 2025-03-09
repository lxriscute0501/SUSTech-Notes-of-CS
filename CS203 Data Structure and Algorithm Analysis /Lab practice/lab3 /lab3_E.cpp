#include <iostream>
#include <cmath>
#include <climits>
using namespace std;

int lower_bound(int arr[], int size, int target) {
    int left = 0, right = size - 1;
    
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] < target) left = mid + 1; else right = mid - 1;
    }
    
    return left;
}

void find(int a[], int h[], int n) {
    int seen[n];
    int seen_size = 0;

    for (int i = n - 1; i >= 0; i--)
    {
        int minn = INT_MAX;
        int up = lower_bound(seen, seen_size, a[i]);
        
        if (up < seen_size) minn = min(minn, abs(seen[up] - a[i]));
        if (up > 0) minn = min(minn, abs(seen[up - 1] - a[i]));

        h[i] = minn;
        int pos = up;
        for (int j = seen_size; j > pos; j--)
            seen[j] = seen[j - 1];
        
        seen[pos] = a[i];
        seen_size ++;
    }
}

int main()
{
    int n;
    cin >> n;
    int a[n];
    for (int i = 0; i < n; i++)
        cin >> a[i];

    int h[n];
    find(a, h, n);

    for (int i = 0; i < n - 1; i++)
        cout << h[i] << " ";

    return 0;
}
