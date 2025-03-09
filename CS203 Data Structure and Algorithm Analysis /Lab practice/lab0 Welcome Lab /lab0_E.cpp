#include <iostream>
using namespace std;

int main()
{
    int n, a, b, c, k, t;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a >> b >> c;
        for (int j = 1; j <= 2*c + 2*b + 1; j++)
        {
            k = 1; t = 0;
            if (j % 2 == 1) {
                while (k <= 2*a + 2*b + 1)
                {
                    if (k + j <= 2*b+1) {
                        cout << '.';
                        k ++;
                        continue;
                    }
                    if (t == 0) {
                        for (int m=1; m<=a; m++)
                            cout << "+-";
                        cout << '+';
                        t = 1;
                        k += 2*a + 1;
                    }
                    
                    if (k <= 2*a + 2*b + 1 and k + j <= 2*a + 2*b + 2*c + 2) {
                        if (k % 2 == 0) cout << '.';
                        else cout << '+';
                        k ++;
                        continue;
                    }
                    
                    if (k <= 2*a + 2*b + 1 and k + j > 2*a + 2*b + 2*c + 2) {
                        cout << '.';
                        k ++;
                        continue;
                    }
                }
            }
            else {
                while (k <= 2*a + 2*b + 1)
                {
                    if (k + j <= 2*b + 1) {
                        cout << '.';
                        k ++;
                        continue;
                    }
                    if (t == 0 and j <= 2*b) {
                        for (int m = 1; m <= a; m++)
                            cout << "/.";
                        cout << '/';
                        t = 1;
                        k += 2*a + 1;
                    }
                    if (t == 0 and j > 2*b) {
                        for (int m = 1; m <= a; m++)
                            cout << "|.";
                        cout << '|';
                        t = 1;
                        k += 2*a + 1;
                    }
                    
                    if (k <= 2*a + 2*b + 1 and k + j <= 2*a + 2*b + 2*c + 2) {
                        if (k % 2 == 0) cout << '/';
                        else cout << '|';
                        k ++;
                        continue;
                    }
                    
                    if (k <= 2*a + 2*b + 1 and k + j > 2*a + 2*b + 2*c + 2) {
                        cout << '.';
                        k ++;
                        continue;
                    }
                }
                
            }
            cout << endl;
        }
    }
}

