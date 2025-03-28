#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct job {
    char name;
    int left;
    int right;
};

int main()
{
    vector<job> jobs;
    vector<char> ans;
    char ch;
    int a, b;
    
    while (cin >> ch >> a >> b) jobs.push_back({ch, a, b});

    // sort by finish time
    sort(jobs.begin(), jobs.end(), [](const auto& j1, const auto& j2) {
        return j1.right < j2.right;
    });
    
    if (jobs.empty()) return 0;
    
    ans.push_back(jobs[0].name);
    int last = jobs[0].right;
    
    for (size_t i = 1; i < jobs.size(); i++)
    {
        if (jobs[i].left >= last) {
            ans.push_back(jobs[i].name);
            last = jobs[i].right;
        }
    }
    
    for (size_t i = 0; i < ans.size(); i++)
        cout << ans[i] << " ";
    
    return 0;
}
