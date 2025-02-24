#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int index;
    Node* last;
    Node* next;
    bool nil;

    Node(int idx) : index(idx), last(nullptr), next(nullptr), nil(false) {}
    Node(bool head, int i) : nil(true), last(nullptr), next(nullptr) {}
};

void exchange(int u, int v, vector<Node*>& a)
{
    Node* ur = a[u]->next;
    Node* vl = a[v]->last;
    a[u]->next = a[v];
    a[v]->last = a[u];
    ur->last = vl;
    vl->next = ur;
}

int main()
{
    int n, m, k, x;
    cin >> n >> m >> k >> x;

    vector<Node*> a(n*m);
    vector<Node*> left(n);
    vector<Node*> right(n);

    int t = 0;
    for (int i=0; i<n; i++)
    {
        left[i] = new Node(true, i);
        right[i] = new Node(false, i);

        vector<Node*> arr(m+2);
        arr[0] = left[i];
        arr[m+1] = right[i];

        for (int j=1; j<=m; j++)
        {
            arr[j] = new Node(t++);
            a[t-1] = arr[j];
        }

        for (int j=1; j<=m; j++)
        {
            arr[j]->last = arr[j-1];
            arr[j]->next = arr[j+1];
        }
        left[i]->next = arr[1];
        right[i]->last = arr[m];
    }

    for (int i=1; i<=k; i++)
    {
        int u, v;
        cin >> u >> v;
        exchange(u, v, a);
    }

    Node* head = a[x];
    while (!head->last->nil) head = head->last;
    while (!head->nil)
    {
        cout << head->index << " ";
        head = head->next;
    }
    
    return 0;
}
