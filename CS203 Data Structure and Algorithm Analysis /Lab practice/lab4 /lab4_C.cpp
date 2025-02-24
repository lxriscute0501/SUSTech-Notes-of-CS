#include <iostream>
#include <deque>
#include <vector>
#include <sstream>

using namespace std;

struct Deque {
    deque<int> data; // the actual deque
    bool isReversed; // flag to indicate if the deque is reversed

    Deque() : isReversed(false) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;

    while (getline(cin, line)) {
        stringstream ss(line);
        int n, q;
        ss >> n >> q;

        vector<Deque> deques(n + 1); // using 1-based index for deques

        while (q--) {
            getline(cin, line);
            stringstream command(line);
            int type, u, v, w, val;

            command >> type;
            if (type == 1) {
                command >> u >> w >> val;
                if (w == 0) {
                    deques[u].data.push_front(val); // insert at front
                } else {
                    deques[u].data.push_back(val); // insert at rear
                }
            } else if (type == 2) {
                command >> u >> w;
                if (w == 0) {
                    if (deques[u].data.empty()) {
                        cout << -1 << '\n'; // empty deque
                    } else {
                        cout << deques[u].data.front() << '\n'; // query front
                        deques[u].data.pop_front(); // pop front
                    }
                } else {
                    if (deques[u].data.empty()) {
                        cout << -1 << '\n'; // empty deque
                    } else {
                        cout << deques[u].data.back() << '\n'; // query rear
                        deques[u].data.pop_back(); // pop back
                    }
                }
            } else if (type == 3) {
                command >> u >> v >> w;
                if (w == 0) {
                    // Direct connection
                    if (!deques[v].data.empty()) {
                        deques[u].data.insert(deques[u].data.end(), deques[v].data.begin(), deques[v].data.end());
                    }
                } else {
                    // Reversed connection
                    if (!deques[v].data.empty()) {
                        deques[u].data.insert(deques[u].data.end(), deques[v].data.rbegin(), deques[v].data.rend());
                    }
                }
                deques[v].data.clear(); // clear deque v after connection
            }
        }
    }

    return 0;
}
