#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int to;
    int w;
    Edge(int t, int w) : to(t), w(w) {}
};

class MinHeap {
public:
    vector<int> heap;
    vector<long long> v;
    vector<int> position;

    MinHeap(int n) {
        heap.reserve(n);
        position.resize(n+1, -1);
    }

    void insert(int city, long long dist) {
        heap.push_back(city);
        v.push_back(dist);
        position[city] = heap.size() - 1;
        weihu_up(heap.size() - 1);
    }

    int fmin1() {
        if (heap.empty()) return -1;
        return heap[0];
    }

    long long fmin2() {
        if (heap.empty()) return LLONG_MAX;
        return v[0];
    }

    void de() {
        position[heap[0]] = -1;
        swap(heap[0], heap[heap.size() - 1]);
        swap(v[0], v[heap.size() - 1]);
        position[heap[0]] = 0;
        heap.pop_back();
        v.pop_back();
        weihu_down(0);
    }

    void decreaseKey(int city, long long newDist) {
        int idx = position[city];
        if (idx == -1) return;
        v[idx] = newDist;
        weihu_up(idx);
    }

    bool empty() {
        return heap.empty();
    }

private:
    void weihu_up(int idx) {
        while (idx > 0 && v[idx] < v[(idx - 1) / 2]) {
            swap(heap[idx], heap[(idx - 1) / 2]);
            swap(v[idx], v[(idx - 1) / 2]);
            position[heap[idx]] = idx;
            position[heap[(idx - 1) / 2]] = (idx - 1) / 2;
            idx = (idx - 1) / 2;
        }
    }

    void weihu_down(int idx) {
        int minn = idx;
        if (2 * idx + 1 < heap.size() && v[2 * idx + 1] < v[minn])
            minn = 2 * idx + 1;
        if (2 * idx + 2 < heap.size() && v[2 * idx + 2] < v[minn])
            minn = 2 * idx + 2;

        if (minn != idx) {
            swap(heap[idx], heap[minn]);
            swap(v[idx], v[minn]);
            position[heap[idx]] = idx;
            position[heap[minn]] = minn;
            weihu_down(minn);
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> graph(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        graph[u].push_back(Edge(v, w));
    }

    vector<long long> dist(n + 1, LLONG_MAX);
    dist[1] = 0;

    MinHeap minheap(n);
    minheap.insert(1, 0);

    while (!minheap.empty()) {
        int u = minheap.fmin1();
        long long d = minheap.fmin2();
        minheap.de();

        if (d > dist[u]) continue;

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            long long weight = edge.w;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                if (minheap.position[v] == -1) {
                    minheap.insert(v, dist[v]);
                } else {
                    minheap.decreaseKey(v, dist[v]);
                }
            }
        }
    }

    if (dist[n] == LLONG_MAX) {
        cout << -1 << endl;
    } else {
        cout << dist[n] << endl;
    }

    return 0;
}
