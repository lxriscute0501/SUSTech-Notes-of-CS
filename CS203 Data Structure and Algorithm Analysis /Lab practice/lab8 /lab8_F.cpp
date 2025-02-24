#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int t;
    long long a[100001], b[100001];
    cin >> t;
    
    while (t --)
    {
        long long n, m, sum = 0;
        cin >> n >> m;
        for (long long i=1; i<=n; i++)
            cin >> a[i] >> b[i];
        
        vector<long long> g[n+1];
        long long rudu[n+1], f[n+1], q[n+1];
        for (long long i=0; i<=n; i++)
        {
            rudu[i] = f[i] = q[i] = 0;
        }
        for (long long i=0; i<m; i++)
        {
            long long u, v;
            cin >> u >> v;
            g[u].push_back(v);
            rudu[v] ++;
        }

        long long front = 0, rear = 0;
        for (long long i=1; i<=n; i++)
        {
            if (rudu[i] == 0) q[rear++] = i;
        }

        while (front < rear)
        {
            long long u = q[front++];
            
            for (long long v : g[u])
            {
                f[v] = (f[v] + f[u] + a[u]) % 1000000007;
                rudu[v] --;
                if (rudu[v] == 0) q[rear++] = v;
            }
        }

        for (long long i=1; i<=n; i++)
            sum = (sum + f[i] * b[i]) % 1000000007;
        cout << sum << endl;
    }

    return 0;
}
