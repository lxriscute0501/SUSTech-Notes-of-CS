#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int sum = INT_MIN;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int maxpath(TreeNode* root) {
    if (root == nullptr) return 0;
    int l = max(0, maxpath(root->left));
    int r = max(0, maxpath(root->right));
    sum = max(sum, root->val + l + r);
    return root->val + max(l, r);
}

int main()
{
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    
    vector<TreeNode*> node(n + 1);
    for (int i = 1; i <= n; i++)
        node[i] = new TreeNode(a[i]);
    
    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        if (node[u]->left == nullptr) node[u]->left = node[v]; else node[u]->right = node[v];
    }
    
    maxpath(node[1]);
    cout << sum << endl;
    return 0;
}
