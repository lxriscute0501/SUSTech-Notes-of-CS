#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Player {
    int a;
    int b;
};

void merge(vector<Player>& arr, int l, int r)
{
    int left = l;
    int mid = (l + r) / 2;
    int m = mid + 1;

    vector<Player> a;
    a.reserve(r-l+1);

    while (l <= mid and m <= r)
    {
        if (arr[l].b >= arr[m].b) a.push_back(arr[m ++]);
        else a.push_back(arr[l ++]);
    }

    while (l <= mid) a.push_back(arr[l ++]);
    while (m <= r) a.push_back(arr[m ++]);

    for (int i = 0; i < a.size(); i++)
        arr[left+i] = a[i];
}

void merge_sort(vector<Player>& arr, int l, int r)
{
    if (l >= r) return;
    int mid = (l + r) / 2;
    merge_sort(arr, l, mid);
    merge_sort(arr, mid + 1, r);
    merge(arr, l, r);
}


int main()
{
    int t;
    cin >> t;
    for (int j=1; j<=t; j++)
    {
        int n;
        scanf("%d", &n);
        vector<Player> players(n);
        
        for (int i = 0; i < n; i++)
            scanf("%d", &players[i].a);
        for (int i = 0; i < n; i++)
            scanf("%d", &players[i].b);

        merge_sort(players, 0, n - 1);

        vector<int> heap(n+1);
        int rear = 1, idx = n-1, ans = 0;
        int t = players[n - 1].b;

        while (t > 0)
        {
            if (idx >= 0 and t == players[idx].b)
            {
                int x = rear;
                heap[rear ++] = players[idx].a;

                while (x > 1)
                {
                    if (heap[x/2] < heap[x]) {
                        swap(heap[x], heap[x/2]);
                        x /= 2;
                    } else break;
                }
                idx --;
                continue;
            }

            if (rear > 1)
            {
                ans += heap[1];
                heap[1] = heap[-- rear];
                int x=1;
                while (2 * x < rear)
                {
                    if (2 * x + 1 < rear) {
                        if (heap[x] < heap[2*x] or heap[x] < heap[2*x+1])
                        {
                            if (heap[2*x] > heap[2*x+1]) {
                                swap(heap[x], heap[2*x]);
                                x *= 2;
                            } else {
                                swap(heap[x], heap[2*x+1]);
                                x = x*2+1;
                            }
                        } else break;
                    } else {
                        if (heap[x] < heap[2*x]) swap(heap[x], heap[2*x]);
                        break;
                    }
                }
            }
            t --;
        }
        printf("%d\n", ans);
    }
    return 0;
}
