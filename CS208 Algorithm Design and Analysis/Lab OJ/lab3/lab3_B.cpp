#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int l, r;
    bool type;
};

int main()
{
    int n, m;
    cin >> n >> m;

    vector<Node> node(n + m);
    for (int i = 0; i < n; i++)
    {
        cin >> node[i].l >> node[i].r;
        node[i].type = false;
    }
    
    for (int i = n; i < n + m; i++)
    {
        cin >> node[i].l >> node[i].r;
        node[i].type = true;
    }
    
    sort(node.begin(), node.end(), [](const Node &a, const Node &b) {
            if (a.l == b.l) return a.type > b.type; else return a.l < b.l;
        });
    
    int ans = 0, ed = 0;
    for (int i = 0; i < n + m; i++)
    {
        if (node[i].type) {
            ed = max(ed, node[i].r);
        } else {
            if (node[i].r <= ed) ans ++;
        }
    }
    
    cout << ans;
    return 0;
}
