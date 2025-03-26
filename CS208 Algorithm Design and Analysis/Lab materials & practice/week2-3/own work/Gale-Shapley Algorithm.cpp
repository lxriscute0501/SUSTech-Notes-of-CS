#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

void stable_matching(int n, vector<string>& men, vector<string>& women,
                     unordered_map<string, vector<string>>& men_pref,
                     unordered_map<string, vector<string>>& women_pref) {
    unordered_map<string, int> men_map, women_map;
    for (int i = 0; i < n; i++)
    {
        men_map[men[i]] = i;
        women_map[women[i]] = i;
    }

    vector<vector<int>> men_pref_idx(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            men_pref_idx[i][j] = women_map[men_pref[men[i]][j]];

    vector<vector<int>> women_rank(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            women_rank[i][women_map[women_pref[women[i]][j]]] = j;

    vector<int> men_to_women(n, -1);
    vector<int> women_to_men(n, -1);

    queue<int> men_free;
    for (int i = 0; i < n; i++)
        men_free.push(i);

    while (!men_free.empty())
    {
        int man = men_free.front();
        men_free.pop();

        for (int woman_idx : men_pref_idx[man])
        {
            if (women_to_men[woman_idx] == -1) {
                men_to_women[man] = woman_idx;
                women_to_men[woman_idx] = man;
                break;
            } else {
                int current_man = women_to_men[woman_idx];
                if (women_rank[woman_idx][man] < women_rank[woman_idx][current_man]) {
                    men_to_women[current_man] = -1;
                    men_free.push(current_man);
                    men_to_women[man] = woman_idx;
                    women_to_men[woman_idx] = man;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
        cout << men[i] << " " << women[men_to_women[i]] << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> men(n), women(n);
    for (int i = 0; i < n; i++)
        cin >> men[i];
    for (int i = 0; i < n; i++)
        cin >> women[i];

    unordered_map<string, vector<string>> men_pref;
    for (int i = 0; i < n; i++)
    {
        men_pref[men[i]] = vector<string>(n);
        for (int j = 0; j < n; j++)
            cin >> men_pref[men[i]][j];
    }

    unordered_map<string, vector<string>> women_pref;
    for (int i = 0; i < n; i++)
    {
        women_pref[women[i]] = vector<string>(n);
        for (int j = 0; j < n; j++)
            cin >> women_pref[women[i]][j];
    }

    stable_matching(n, men, women, men_pref, women_pref);
    return 0;
}
