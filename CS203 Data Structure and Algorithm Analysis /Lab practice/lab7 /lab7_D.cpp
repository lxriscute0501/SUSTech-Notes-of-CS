#include <iostream>
#include <vector>
using namespace std;

void insert(vector<long long>& heap, int k, long long a, int& num)
{
    if (num <= k)
    {
        heap[num] = a;
        int h = num;
        while (h > 1)
        {
            if (heap[h/2] > heap[h]) {
                swap(heap[h/2], heap[h]);
                h /= 2;
            } else break;
        }
        return;
    }
    if (a <= heap[1]) return;

    heap[1] = a;
    int h=1;
    while (2*h <= k)
    {
        if (2*h+1 <= k) {
            if (heap[2*h] < heap[h] or heap[2*h+1] < heap[h])
            {
                if (heap[2*h] < heap[2*h+1]) {
                    swap(heap[h], heap[2*h]);
                    h *= 2;
                } else {
                    swap(heap[h], heap[2*h+1]);
                    h = 2*h+1;
                }
            } else break;
        } else if (heap[2*h] < heap[h])
        {
            swap(heap[h], heap[2*h]);
            break;
        } else break;
    }
}

int main()
{
    int t, k;
    long long s;
    cin >> t >> k >> s;

    int T = t / 100;
    int ans = 0;
    vector<long long> heap(k+1, 0);

    for (int i=0; i<T; i++)
    {
        long long a = 0;
        for (int j=1; j<=100; j++)
        {
            long long n = 100 * i + j + s;
            a = n;
            while (n >= 1)
            {
                a += n % 10;
                n /= 10;
            }
            if (ans <= k) ans ++;
            insert(heap, k, a, ans);
        }

        long long  result = heap[1];
        s = result;
        cout << result << " ";
    }

    return 0;
}
