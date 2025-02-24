#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void mergesort(vector<long>& a, int l, int r)
{
    if (l >= r) return;
    int mid = (l + r) / 2;
    mergesort(a, l, mid);
    mergesort(a, mid + 1, r);

    vector<long> temp(r - l + 1);
    int lb = l, rb = mid + 1, k = 0;

    while (lb <= mid && rb <= r)
    {
        if (a[lb] <= a[rb]) {
            temp[k++] = a[lb++];
        } else {
            temp[k++] = a[rb++];
        }
    }

    while (lb <= mid) {
        temp[k++] = a[lb++];
    }
    while (rb <= r) {
        temp[k++] = a[rb++];
    }

    for (int i = 0; i < temp.size(); i++) {
        a[l+i] = temp[i];
    }
}

bool search(const vector<long>& hash, long ha) {
    int l = 0;
    int r = hash.size() - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (hash[mid] == ha) {
            return true;
        } else if (hash[mid] < ha) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return false;
}


long check(const string& a, const string& b, int mid) {
    if (mid == 0) return 0;
    long mo = 1;
    for (int i = 0; i < mid - 1; i++)
        mo *= 139;

    vector<long> hash(b.size() - mid + 1);
    long hb = 0;
    for (int j = 0; j <= b.size() - mid; j++) {
        if (j == 0) {
            for (int k = 0; k < mid; k++) {
                hb = (hb * 139 + b[j + k]);
            }
        } else {
            hb = (hb * 139 + b[j+mid-1] - b[j-1] * mo * 139);
        }
        hash[j] = hb;
    }

    mergesort(hash, 0, b.size() - mid);

    long ha = 0;
    for (int i = 0; i <= a.size() - mid; i++) {
        if (i == 0) {
            for (int j = 0; j < mid; j++) {
                ha = (ha * 139 + a[i+j]);
            }
        } else {
            ha = (ha * 139 + a[i+mid-1] - a[i-1] * mo * 139);
        }

        if (search(hash, ha)) return mid;
    }
    return 0;
}

int main()
{
    string a, b;
    cin >> a >> b;

    int l = 0;
    int r = min(a.size(), b.size());
    int result = 0;

    while (l <= r) {
        int mid = (l + r + 1) / 2;
        int ans = check(a, b, mid);
        if (ans != 0) {
            result = ans;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    cout << result << endl;
    return 0;
}
