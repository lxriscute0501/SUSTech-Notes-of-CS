#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Point {
    double x, y;
};

double close(vector<Point>& p, vector<Point>& temp, int left, int right) {
    if (right - left <= 3) {
        double minn = __DBL_MAX__;
        for (int i = left; i < right - 1; i++)
            for (int j = i + 1; j < right; j++)
                minn = min(minn, sqrt((p[i].x - p[j].x) * (p[i].x - p[j].x) + (p[i].y - p[j].y) * (p[i].y - p[j].y)));
        return minn;
    }

    int mid = (left + right) / 2;
    double lmin = close(p, temp, left, mid), rmin = close(p, temp, mid, right);
    double minn = min(lmin, rmin);

    int j = 0;
    for (int i = left; i < right; i++)
    {
        if (p[i].x >= p[mid].x - minn and p[i].x - minn <= p[mid].x) {
            temp[j] = p[i];
            j++;
        }
    }

    sort(temp.begin(), temp.begin() + j, [](const Point& p1, const Point& p2) {
        return p1.y < p2.y;
    });

    for (int i = 0; i < j - 1; i++)
        for (int k = i + 1; k < j and (temp[k].y - temp[i].y) < minn; k++)
            minn = min(minn, sqrt((temp[i].x - temp[k].x) * (temp[i].x - temp[k].x) + (temp[i].y - temp[k].y) * (temp[i].y - temp[k].y)));
    return minn;
}

int main()
{
    int n;
    cin >> n;
    vector<Point> p(n), temp(n);
    for (int i = 0; i < n; i++)
        cin >> p[i].x >> p[i].y;

    sort(p.begin(), p.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x or (p1.x == p2.x and p1.y < p2.y);
    });

    double result = close(p, temp, 0, n);
    cout << fixed << setprecision(4) << result << endl;
    return 0;
}
