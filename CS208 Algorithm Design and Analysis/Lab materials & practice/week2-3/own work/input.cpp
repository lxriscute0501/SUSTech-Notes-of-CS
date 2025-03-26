#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

vector<string> generateNames(const string& prefix, int n) {
    vector<string> names;
    
    for (int i = 1; i <= n; ++i)
        names.push_back(prefix + to_string(i));
    
    return names;
}

vector<vector<string>> generatePreferences(const vector<string>& names, mt19937& rng) {
    vector<vector<string>> preferences;
    
    for (size_t i = 0; i < names.size(); ++i)
    {
        vector<string> pref = names;
        shuffle(pref.begin(), pref.end(), rng);
        preferences.push_back(pref);
    }
    
    return preferences;
}

int main()
{
    const int N = 1000; //also can be 100, 500, 2000, 4 cases have been tried
    mt19937 rng(random_device{}());

    vector<string> men = generateNames("m", N);
    vector<string> women = generateNames("w", N);

    vector<vector<string>> men_pref = generatePreferences(women, rng);
    vector<vector<string>> women_pref = generatePreferences(men, rng);

    ofstream outFile("input.txt");
    if (!outFile) {
        cerr << "Error: Could not open file input.txt for writing." << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::current_path() / "input.txt";
        cout << "Input file will be saved to: " << filePath << endl;
    
    outFile << N << "\n";
    for (const string& man : men)
        outFile << man << " ";
    outFile << "\n";

    for (const string& woman : women)
        outFile << woman << " ";
    outFile << "\n";

    for (size_t i = 0; i < men.size(); ++i)
    {
        for (const string& woman : men_pref[i]) {
            outFile << woman << " ";
        }
        
        outFile << "\n";
    }

    for (size_t i = 0; i < women.size(); ++i)
    {
        for (const string& man : women_pref[i]) {
            outFile << man << " ";
        }
        
        outFile << "\n";
    }

    outFile.close();
    return 0;
}
