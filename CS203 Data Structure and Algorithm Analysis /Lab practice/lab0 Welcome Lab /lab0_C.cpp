#include <iostream>
#include <string>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void swap(string &a, string &b) {
    string temp = a;
    a = b;
    b = temp;
}

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        string a[14];
        int b[14] = {0};

        for (int j = 1; j <= 13; j++)
            cin >> a[j];

        for (int j = 1; j <= 13; j++)
        {
            if (a[j].length() == 1) {
                switch (a[j][0])
                {
                    case 'E': b[j] = 40; break;
                    case 'S': b[j] = 50; break;
                    case 'W': b[j] = 60; break;
                    case 'N': b[j] = 70; break;
                    case 'B': b[j] = 80; break;
                    case 'F': b[j] = 90; break;
                    case 'Z': b[j] = 100; break;
                }
            } else if (a[j].length() == 2) {
                switch (a[j][0])
                {
                    case 'W':
                        b[j] = 10 + (a[j][1] - '0');
                        break;
                    case 'T':
                        b[j] = 20 + (a[j][1] - '0');
                        break;
                    case 'Y':
                        b[j] = 30 + (a[j][1] - '0');
                        break;
                }
            }
        }

        for (int j = 1; j <= 12; j++)
            for (int k = j + 1; k <= 13; k++)
                if (b[j] > b[k]) {
                    swap(b[j], b[k]);
                    swap(a[j], a[k]);
                }

        for (int j = 1; j <= 13; j++)
            cout << a[j] << ' ';
        cout << endl;
    }
    return 0;
}
