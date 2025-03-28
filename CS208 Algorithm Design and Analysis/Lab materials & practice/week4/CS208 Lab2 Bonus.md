# CS208 Lab2 Bonus

12312110 李轩然

<br>

## Description

**Hand write the DFS code and indicate at which different positions the output would produce pre-order, in-order, and post-order traversal results.**


## C++ Code
```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<int> tree;

//pre-order
void dfs_pre(int idx) {
    if (idx >= tree.size() or tree[idx] == -1) return;
    cout << tree[idx] << " ";
    dfs_pre(2 * idx + 1);
    dfs_pre(2 * idx + 2);
}

//in-order
void dfs_in(int idx) {
    if (idx >= tree.size() or tree[idx] == -1) return;
    dfs_in(2 * idx + 1);
    cout << tree[idx] << " ";
    dfs_in(2 * idx + 2);
}

//post-order
void dfs_post(int idx) {
    if (idx >= tree.size() or tree[idx] == -1) return;
    dfs_post(2 * idx + 1);
    dfs_post(2 * idx + 2);
    cout << tree[idx] << " ";
}

int main()
{
    int n, x;
    cin >> n;
    
    for (int i = 0; i < n; i++)
    {
        cin >> x;
        tree.push_back(x);
    }
    
    dfs_pre(0);
    cout<<endl;
    dfs_in(0);
    cout<<endl;
    dfs_post(0);
    return 0;
}

```