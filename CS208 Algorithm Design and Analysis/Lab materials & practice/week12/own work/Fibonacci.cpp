class Solution {
public:
    int Fibonacci(int n) {
        int a[n + 1];
        a[1] = 1; a[2] = 1;
        
        for (int i=3; i<=n; i++)
        a[i] = a[i-1] + a[i-2];

        return a[n];
    }
};