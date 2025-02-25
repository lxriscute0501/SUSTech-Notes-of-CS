#include <iostream>
#include <vector>
#include <climits>
using namespace std;

long long sum = 0;
struct Edge {
    int u, v, w;
    Edge(int u, int v, int w) : u(u), v(v), w(w) {}
};

class MinHeap {
public:
    vector<pair<long long, int>> heap;
    vector<int> pos;

    MinHeap(int n)
    {
        heap.reserve(n+5);
        pos.resize(n+5, -1);
    }

    void push(long long weight, int node)
    {
        heap.push_back({weight, node});
        pos[node] = heap.size() - 1;
        weihu_up(heap.size() - 1);
    }

    pair<long long, int> pop()
    {
        if (heap.size() == 0) return {-1, -1};
        pair<long long, int> result = heap[0];
        swap(heap[0], heap[heap.size() - 1]);
        pos[heap[0].second] = 0;
        heap.pop_back();
        weihu_down(0);
        return result;
    }

    void update(int node, long long new_weight)
    {
        int index = pos[node];
        if (index == -1) {
            push(new_weight, node);
            return;
        }
        heap[index].first = new_weight;
        weihu_up(index);
    }

    bool empty()
    {
        return heap.empty();
    }

private:
    void weihu_up(int idx)
    {
        while (idx > 0)
        {
            int parent = (idx - 1) / 2;
            if (heap[parent].first <= heap[idx].first) break;
            swap(heap[idx], heap[parent]);
            pos[heap[parent].second] = parent;
            pos[heap[idx].second] = idx;
            idx = parent;
        }
    }

    void weihu_down(int index)
    {
        while (2 * index + 1 < heap.size())
        {
            int minn = index;
            if (2 * index + 1 < heap.size() and heap[2 * index + 1].first < heap[minn].first) minn = 2 * index + 1;
            if (2 * index + 2 < heap.size() and heap[2 * index + 2].first < heap[minn].first) minn = 2 * index + 2;
            if (minn == index) break;
            swap(heap[index], heap[minn]);
            pos[heap[index].second] = index;
            pos[heap[minn].second] = minn;
            index = minn;
        }
    }
};

class Graph {
public:
    int n, m;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;

    Graph(int n, int m) : n(n), m(m), adj(n + 5) {}

    void addEdge(int u, int v, int w)
    {
        edges.push_back(Edge(u, v, w));
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
        sum += w;
    }

    long long prim()
    {
        long long w = 0;
        vector<bool> mst(n+1, false);
        vector<long long> minEdge(n+1, LLONG_MAX / 2); // 避免溢出
        minEdge[1] = 0;

        MinHeap minHeap(n);
        minHeap.push(0, 1);

        while (!minHeap.empty())
        {
            auto top = minHeap.pop();
            long long weight = top.first;
            int u = top.second;

            if (mst[u]) continue;
            mst[u] = true;
            w += weight;

            for (auto& neighbor : adj[u])
            {
                int v = neighbor.first;
                int weight_v = neighbor.second;
                if (!mst[v] and weight_v < minEdge[v]) {
                    minEdge[v] = weight_v;
                    minHeap.update(v, weight_v);
                }
            }
        }
        return w;
    }
};

int main()
{
    int n, m;
    cin >> n >> m;
    sum = 0;
    Graph g(n, m);

    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    
    long long mstWeight = g.prim();
    cout << sum - mstWeight << endl;
    return 0;
}
