#include <iostream>
using namespace std;

const int MOD = 998244353;
long long c[1001], l[1001], dp[1001];
int num[1001];

long long mpow(long long base, long long exp)
{
    long long r = 1;
    while (exp != 0)
    {
        if (exp % 2 == 1) r = r * base % MOD;
        base = base * base % MOD;
        exp /= 2;
    }
    return r;
}

int main()
{
    int N;
    cin >> N;
    c[0] = 1; l[0] = 1;
    for (int i=1; i<=N; i++)
    {
        c[i] = c[i-1] * i % MOD;
        l[i] = mpow(c[i], MOD-2);
    }

    for (int i=N; i>0; i--)
    {
        if (2*i+1 <= N) num[i] += num[2*i+1] + num[2*i] + 1;
        else if (2*i <= N) num[i] += num[2*i] + 1;
        else num[i] = 1;
    }

    for (int i=N; i>0; i--)
    {
        if (2*i+1 <= N) dp[i] = dp[2*i+1] * dp[2*i] % MOD * (c[num[i]-1] * l[num[2*i]] % MOD * l[num[i]-num[2*i]-1] % MOD) % MOD;
        else dp[i] = 1;
    }
    cout << dp[1] << endl;
    return 0;
}
