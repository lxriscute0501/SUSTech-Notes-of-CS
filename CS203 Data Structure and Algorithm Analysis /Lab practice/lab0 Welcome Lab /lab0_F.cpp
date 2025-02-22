#include <iostream>
using namespace std;

int n, w[10], s[10], b[10], z[10], x, y;
string t;

//判断是否能和牌
bool panduan()
{
    for (int i=1; i<=7; i++)
        if (z[i] > 0) return false;
    
    if (y == 0) return true;
    
    for (int i=1; i<=9; i++)
    {
        if (w[i] >= 3) {
            w[i] -= 3;
            y -= 3;
            if (panduan()) return true;
            panduan();
            w[i] += 3;
            y += 3;
        }
        if (b[i] >= 3) {
            b[i] -= 3;
            y -= 3;
            if (panduan()) return true;
            panduan();
            b[i] += 3;
            y += 3;
        }
        if (s[i] >= 3) {
            s[i] -= 3;
            y -= 3;
            if (panduan()) return true;
            panduan();
            s[i] += 3;
            y += 3;
        }
        if (w[i] >= 1 and w[i+1] >= 1 and w[i+2] >= 1 and i <= 7) {
            w[i] --; w[i+1] --; w[i+2] --; y -= 3;
            if (panduan()) return true;
            w[i] ++; w[i+1] ++; w[i+2] ++; y += 3;
        }
        if (b[i] >= 1 and b[i+1] >= 1 and b[i+2] >= 1 and i <= 7) {
            b[i] --; b[i+1] --; b[i+2] --; y -= 3;
            if (panduan()) return true;
            b[i] ++; b[i+1] ++; b[i+2] ++; y += 3;
        }
        if (s[i] >= 1 and s[i+1] >= 1 and s[i+2] >= 1 and i <= 7) {
            s[i] --; s[i+1] --; s[i+2] --; y -= 3;
            if (panduan()) return true;
            s[i] ++; s[i+1] ++; s[i+2] ++; y += 3;
        }
    }
    return false;
}


int main()
{
    cin >> n;
    for (int i=1; i<=n; i++)
    {
        x = 0; y = 14;
        cin >> t;
        
        for (int j=1; j<=9; j++)
        {
            w[j] = 0; b[j] = 0; s[j] = 0; z[j] = 0;
        }
        
        for (int j=1; j<=14; j++)
        {
            if (t[j*2-1] == 'w') w[t[j*2-2] - '0'] ++;
            if (t[j*2-1] == 'b') b[t[j*2-2] - '0'] ++;
            if (t[j*2-1] == 's') s[t[j*2-2] - '0'] ++;
            if (t[j*2-1] == 'z') z[t[j*2-2] - '0'] ++;
        }
        
        for (int j=1; j<=7; j++)
        {
            if (z[j] >= 3) {
                y -= (z[j] / 3) * 3;
                z[j] %= 3;
            }
        }
        
        for (int j=1; j<=9; j++)
        {
            if (w[j] >= 2) {
                w[j] -= 2; y -= 2;
                if (panduan()) {
                    cout << "Blessing of Heaven" << endl;
                    x = 1;
                    break;
                } else {
                    w[j] += 2;
                    y += 2;
                }
            }
        }
        if (x == 1) continue;
        
        for (int j=1; j<=9; j++)
        {
            if (b[j] >= 2) {
                b[j] -= 2; y -= 2;
                if (panduan()) {
                    cout << "Blessing of Heaven" << endl;
                    x = 1;
                    break;
                } else {
                    b[j] += 2;
                    y += 2;
                }
            }
        }
        if (x == 1) continue;
        
        for (int j=1; j<=9; j++)
        {
            if (s[j] >= 2) {
                s[j] -= 2; y -= 2;
                if (panduan()) {
                    cout << "Blessing of Heaven" << endl;
                    x = 1;
                    break;
                } else {
                    s[j] += 2;
                    y += 2;
                }
            }
        }
        if (x == 1) continue;
        
        for (int j=1; j<=7; j++)
        {
            if (z[j] >= 2) {
                z[j] -= 2; y -= 2;
                if (panduan()) {
                    cout << "Blessing of Heaven" << endl;
                    x = 1;
                    break;
                } else {
                    z[j] += 2;
                    y += 2;
                }
            }
        }
        if (x == 0) cout << "Bad luck" << endl;
    }
    return 0;
}
