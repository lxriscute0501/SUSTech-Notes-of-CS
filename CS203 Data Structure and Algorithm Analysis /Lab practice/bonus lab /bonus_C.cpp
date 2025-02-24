#include <iostream>
using namespace std;

int main()
{
    int n, k;
    char qr;
    string s[1001], ss[1001];
    bool b[1001];
    
    cin >> n >> k;
    
    int front = 0, rear = 1, count = 1;
    b[0] = true;
    for (int i=0; i<256; i++)
    {
        cin >> qr;
        s[0] += qr;
    }
    ss[0] = s[0];
    cout << "miss" <<endl;
    
    for (int i=1; i<n; i++)
    {
        bool x = true;
        for (int j=0; j<256; j++)
        {
            cin >> qr;
            s[i] += qr;
        }
        
        for (int j = front; j < rear; j++)
        {
            if (s[i] == ss[j] and b[j]) {
                b[rear] = true;
                ss[rear] = s[i];
                rear ++;
                b[j] = false;
                x = false;
                break;
            }
        }
        
        while (!b[front]) front ++;
        
        if (x) {
            if (count >= k) front ++;
            ss[rear] = s[i];
            b[rear] = true;
            rear ++;
            count ++;
            cout << "miss" << endl;
        } else cout << "hit" << endl;
    }

    return 0;
}
