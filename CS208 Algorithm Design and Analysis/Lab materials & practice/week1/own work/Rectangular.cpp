#include <iostream>
using namespace std;

int count(int n) {
    if (n <= 2) return n;
    return count(n - 1) + count (n - 2);
}

int main()
{
    int n;
    cin >> n;

    if (n < 1) cout << "Invalid input!";
    else {
        int cnt = count(n);
        cout << cnt;
    }

    return 0;
}
