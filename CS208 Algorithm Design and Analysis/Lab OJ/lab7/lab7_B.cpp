#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> conquer(const vector<vector<int>>& left, const vector<vector<int>>& right) {
    vector<vector<int>> result;
    int i = 0, j = 0;
    int leftH = 0, rightH = 0, h = 0;
    
    while (i < left.size() and j < right.size())
    {
        if (left[i][0] < right[j][0]) {
            leftH = left[i][1];
            int temp = max(leftH, rightH);
            if (temp != h) {
                h = temp;
                result.push_back({left[i][0], h});
            }
            i++;
        }
        else if (left[i][0] > right[j][0]) {
            rightH = right[j][1];
            int temp = max(leftH, rightH);
            if (temp != h) {
                h = temp;
                result.push_back({right[j][0], h});
            }
            j++;
        }
        else {
            leftH = left[i][1];
            rightH = right[j][1];
            int temp = max(leftH, rightH);
            if (temp != h) {
                h = temp;
                result.push_back({left[i][0], h});
            }
            i++; j++;
        }
    }
    
    while (i < left.size())
    {
        if (result.empty() or left[i][1] != result.back()[1]) {
            result.push_back(left[i]);
        }
        i++;
    }

    while (j < right.size())
    {
        if (result.empty() or right[j][1] != result.back()[1]) {
            result.push_back(right[j]);
        }
        j++;
    }
    
    return result;
}

vector<vector<int>> divide(vector<vector<int>>& rec) {
    if (rec.size() == 0) return {};
    if (rec.size() == 1) return {{rec[0][0], rec[0][2]}, {rec[0][1], 0}};
    
    int mid = (int)rec.size() / 2;
    vector<vector<int>> lrec(rec.begin(), rec.begin() + mid);
    vector<vector<int>> rrec(rec.begin() + mid, rec.end());
    
    auto left = divide(lrec);
    auto right = divide(rrec);
    return conquer(left, right);
}

int main()
{
    int n;
    cin >> n;
    
    vector<vector<int>> rec(n, vector<int>(3));
    for (int i = 0; i < n; i++)
        cin >> rec[i][0] >> rec[i][1] >> rec[i][2];
        
    sort(rec.begin(), rec.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });
    
    auto result = divide(rec);
    
    if (!result.empty() and result.back()[1] != 0) result.push_back({result.back()[0], 0});
    
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i][0] << " " << result[i][1] << endl;
    }
    
    return 0;
}
