#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
using namespace std;

string s;

string subs(int left, int right) {
    string ss = "";
    for (int i = left; i < right; i++)
        ss += s[i];
    return ss;
}

int divide(string s, int k) {
    unordered_map<char, int> umap;
    
    for (char c : s)
        umap[c] ++;
    
    vector<int> split;
    
    for (int i = 0; i < s.length(); i++)
        if (umap[s[i]] < k) split.push_back(i);

    if (split.size() == 0) return (int)s.length();
    
    int ans = 0, left = 0;
    split.push_back((int)s.length());
    
    for (int i = 0; i < split.size(); i++)
    {
        if (split[i] - left > ans) ans = max(ans, divide(subs(left, split[i]), k));
        left = split[i] + 1;
    }
    
    return ans;
}

int main()
{
    int k;
    cin >> s >> k;
    cout << divide(s, k);
    return 0;
}