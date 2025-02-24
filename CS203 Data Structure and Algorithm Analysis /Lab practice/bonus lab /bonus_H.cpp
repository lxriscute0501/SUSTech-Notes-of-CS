#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
double t[301][301], speed[301][301];
int height[301][301];

struct Node {
    int x, y;
    double time;
    double v;
};

Node heap[90001];
int sz = 0;

void weihu(Node node)
{
    heap[sz++] = node;
    int index = sz-1;
    
    while (index > 0)
    {
        if (heap[(index - 1) / 2].time <= heap[index].time) break;
        swap(heap[(index - 1) / 2], heap[index]);
        index = (index - 1) / 2;
    }
}

Node pop()
{
    Node top = heap[0];
    heap[0] = heap[--sz];
    int index = 0;
    while (index*2+1 < sz)
    {
        int left = index*2+1, right = index*2+2;
        int minn = index;
        if (left < sz and heap[left].time < heap[minn].time) minn = left;
        if (right < sz and heap[right].time < heap[minn].time) minn = right;
        if (minn == index) break;
        swap(heap[index], heap[minn]);
        index = minn;
    }
    return top;
}

int main()
{
    int n, m;
    cin >> n >> m;

    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            cin >> height[i][j];

    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
        {
            t[i][j] = 1e9;
            speed[i][j] = 0.0;
        }
    
    t[0][0] = 0.0;
    speed[0][0] = 1.0;
    weihu({0, 0, 0.0, 1.0});

    while (true)
    {
        Node cur = pop();
        int x = cur.x, y = cur.y;
        double ntime = cur.time;
        double nspeed = cur.v;

        if (x == n - 1 and y == m - 1) {
            printf("%.2f\n", ntime);
            return 0;
        }

        for (int i=0; i<4; i++)
        {
            int nx = x + dx[i], ny = y + dy[i];

            if (nx >= 0 and nx < n and ny >= 0 and ny < m) {
                double mt = 1.0 / nspeed;
                double newspeed = nspeed * pow(2, height[x][y] - height[nx][ny]);
                double newtime = ntime + mt;
                
                if (newtime < t[nx][ny]) {
                    t[nx][ny] = newtime;
                    speed[nx][ny] = newspeed;
                    weihu({nx, ny, newtime, newspeed});
                }
            }
        }
    }
    return 0;
}
