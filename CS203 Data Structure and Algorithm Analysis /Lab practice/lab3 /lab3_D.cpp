#include <iostream>
#include <list>
#include <string>
using namespace std;

int main()
{
    int t;
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        int n;
        string s;
        cin >> n;
        cin >> s;
        
        list<char> line;
        auto cursor = line.begin();
        bool rep = false;

        for (char ch : s)
        {
            if (ch == 'I') {
                cursor = line.begin();
            } else if (ch == 'H') {
                if (cursor != line.begin()) cursor --;
            } else if (ch == 'L') {
                if (cursor != line.end()) cursor ++;
            } else if (ch == 'x') {
                if (cursor != line.end()) cursor = line.erase(cursor);
            } else if (ch == 'r') rep = true;
            else  {
                if (rep)
                {
                    if (cursor != line.end() or line.size() == 0) {
                        *cursor = ch;
                    } else {
                        line.insert(cursor, ch);
                    }
                    rep = false;
                } else {
                    line.insert(cursor, ch);
                }
            }
        }
        for (char ch : line) cout << ch;
        cout<<endl;
    }

    return 0;
}
