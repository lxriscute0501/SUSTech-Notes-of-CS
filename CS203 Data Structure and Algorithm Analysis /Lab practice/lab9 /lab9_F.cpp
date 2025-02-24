#include <iostream>
#include <vector>
using namespace std;

struct E {
    int v, w;
};

int n, m, p, k;
bool vis[50001][12];
long long dist[50001][12];
vector<E> g[50001];
vector<int> portal[50001];

struct Node {
    int u, lvl;
    long long d;
    bool operator < (const Node& rhs) const {
        return d > rhs.d;
    }
};

class MinHeap {
public:
    vector<Node> heap;
    vector<int> pos;

    MinHeap(int n) {
        heap.reserve(n + 1);
        pos.resize(n + 1, -1);
    }

    void push(Node node)
    {
        heap.push_back(node);
        pos[node.u] = heap.size() - 1;
        up(heap.size() - 1);
    }

    Node pop() {
        if (heap.empty()) return {-1, -1, (long long)1e18};
        Node top = heap[0];
        swap(heap[0], heap[heap.size() - 1]);
        pos[heap[0].u] = 0;
        heap.pop_back();
        down(0);
        return top;
    }

    void update(int u, long long d)
    {
        int index = pos[u];
        heap[index].d = d;
        up(index);
    }

    bool empty()
    {
        return heap.empty();
    }

private:
    void up(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (heap[parent].d <= heap[idx].d) break;
            swap(heap[idx], heap[parent]);
            pos[heap[parent].u] = parent;
            pos[heap[idx].u] = idx;
            idx = parent;
        }
    }

    void down(int idx)
    {
        while (2 * idx + 1 < heap.size())
        {
            int minn = idx;
            if (2 * idx + 1 < heap.size() and heap[2 * idx + 1].d < heap[minn].d) minn = 2 * idx + 1;
            if (2 * idx + 2 < heap.size() and heap[2 * idx + 2].d < heap[minn].d) minn = 2 * idx + 2;
            if (minn == idx) break;
            swap(heap[idx], heap[minn]);
            pos[heap[idx].u] = idx;
            pos[heap[minn].u] = minn;
            idx = minn;
        }
    }
};

int main()
{
    cin>>n>>m>>p>>k;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            dist[i][j] = 1e18;
        }
    }

    for (int i = 1; i <= m; i++)
    {
        int u, v, c;
        cin>>u>>v>>c;
        g[u].push_back((E){v, c});
    }

    for (int i = 1; i <= p; i++)
    {
        int u, v;
        cin>>u>>v;
        portal[u].push_back(v);
    }

    int s, t;
    cin >> s >> t;

    MinHeap heap(n);
    heap.push({s, 0, 0});
    dist[s][0] = 0;

    while (!heap.empty())
    {
        Node cur = heap.pop();
        if (vis[cur.u][cur.lvl]) continue; else vis[cur.u][cur.lvl] = true;
        for (E& e : g[cur.u])
        {
            if (dist[cur.u][cur.lvl] + e.w < dist[e.v][cur.lvl]) {
                dist[e.v][cur.lvl] = e.w + dist[cur.u][cur.lvl];
                if (!vis[e.v][cur.lvl]) heap.push({e.v, cur.lvl, dist[e.v][cur.lvl]});
            }
        }

        for (int v : portal[cur.u])
        {
            if (cur.lvl < k and dist[cur.u][cur.lvl] < dist[v][cur.lvl+1]) {
                dist[v][cur.lvl+1] = dist[cur.u][cur.lvl];
                if (!vis[v][cur.lvl+1]) heap.push({v, cur.lvl + 1, dist[v][cur.lvl + 1]});
            }
        }
    }

    long long ans = 1e18;
    for (int i = 0; i <= k; i++)
        if (dist[t][i] < ans) ans = dist[t][i];
    cout << ans << endl;
    return 0;
}
