#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void trans(int N, const vector<int>& a)
{
    vector<vector<int>> tree(N + 1);
    
    for (int i=0; i<N-1; i++)
    {
        int p = a[i];
        tree[p].push_back(i + 2);
    }
    vector<int> result;

    int front = 0, rear = 0;
    int queue[N];
    queue[rear++] = 1;

    while (front < rear)
    {
        int current = queue[front++];
        result.push_back(current);
        vector<int>& children = tree[current];
        for (int child : children)
            queue[rear++] = child;
    }

    for (int i = 0; i < result.size(); i++)
        cout << result[i] << " ";
    cout << endl;
}

int main()
{
    int T;
    cin >> T;

    for (int j=1; j<=T; j++)
    {
        int N;
        cin >> N;
        vector<int> a(N - 1);
        
        for (int i=0; i<N-1; i++)
            cin >> a[i];

        trans(N, a);
    }
    return 0;
}
