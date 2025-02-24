#include <iostream>
using namespace std;

int main()
{
    int t;
    string s;
    cin >> t;
    for (int i = 1; i <= t; i++)
    {
        cin >> s;
        cout << s.size() * (s.size() + 1) / 2 << endl;
    }
    return 0;
}
