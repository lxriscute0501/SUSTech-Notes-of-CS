#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <map>
using namespace std;

struct file_system {
    string name;
    string content;
    bool file;
    map<string, file_system*> children;
};

file_system root;
string n, ty;
vector<string> fres;

vector<string> split(const string& s) {
    string x;
    vector<string> res;
    
    for (char ch : s)
    {
        if (!x.empty() and ch == '/') {
                res.push_back(x);
                x.clear();
        } else x += ch;
    }
    
    if (!x.empty()) res.push_back(x);
    return res;
}

file_system* search(const vector<string>& path, bool bo) {
    file_system* temp = &root;
    stack<file_system*> par;

    for (size_t i = 0; i < path.size() - bo; i++)
    {
        if (path[i] == ".") continue;
        if (path[i] == "..") {
            if (!par.empty()) {
                temp = par.top();
                par.pop();
            }
            continue;
        }
        if (temp->children.end() != temp->children.find(path[i])) {
            par.push(temp);
            temp = temp->children[path[i]];
        } else return nullptr;
    }
    
    return temp;
}

void echo(const vector<string>& paras) {
    string content;
    vector<string> path;
    for (size_t i = 1; i < paras.size(); i++)
    {
        if (paras[i] == ">") {
            path = split(paras[i + 1]);
            break;
        } else {
            if (i + 1 < paras.size() and paras[i + 1] == ">") content += paras[i]; else content += paras[i] + " ";
        }
    }
    file_system* temp = search(path, true);
    if (temp) {
        file_system* nfile = new file_system{path.back(), content, true};
        temp->children[path.back()] = nfile;
    }
}

void mkdir(const vector<string>& paras) {
    vector<string> path = split(paras[1]);
    file_system* cur = search(path, true);
    if (cur) {
        file_system* nfolder = new file_system{path.back(), "", false};
        cur->children[path.back()] = nfolder;
    } else return;
}

void rm(const vector<string>& paras) {
    vector<string> path;
    
    for (size_t i = 1; i < paras.size(); i++)
    {
        if (paras[i] != "-rf") path = split(paras[i]);
    }
    file_system* temp = search(path, true);
    if (temp) temp->children.erase(path.back()); else return;
}

void mv(const vector<string>& paras) {
    vector<string> srcpath = split(paras[1]);
    vector<string> dstpath = split(paras[2]);
    file_system* dst = search(dstpath, false);
    file_system* src = search(srcpath, true);
    if (dst and src) {
        dst->children[srcpath.back()] = src->children[srcpath.back()];
        src->children.erase(srcpath.back());
    }
}

void cat(const vector<string>& paras) {
    vector<string> path = split(paras[1]);
    file_system* cur = search(path, true);
    if (cur and cur->children.find(path.back()) != cur->children.end() and cur->children[path.back()]->file) {
        cout << cur->children[path.back()]->content << endl;
    } else cout << "" << endl;
}

void print(file_system* cur, const string& prefix) {
    if (!cur) return;
    bool found = true;
    if (ty == "f" and !cur->file) found = false;
    if (ty == "d" and cur->file) found = false;
    if (!n.empty() and cur->name != n) found = false;
    if (found) fres.push_back(prefix);
    for (auto& child : cur->children) {
        print(child.second, prefix + "/" + child.first);
    }
}

void find(const vector<string>& paras) {
    string prefix;
    fres.clear();
    n = ty = "";
    if (paras.size() > 1 and paras[1].front() != '-') prefix = paras[1]; else prefix = ".";
    vector<string> path = split(prefix);
    
    for (size_t i = 1; i < paras.size(); i++)
    {
        if (paras[i] == "-name") n = paras[++i];
        if (paras[i] == "-type") ty = paras[++i];
    }
    
    if (prefix.back() == '/') prefix.pop_back();
    
    file_system* temp = search(path, false);
    if (temp) print(temp, prefix); else return;
    cout << fres.size() << endl;
    
    for (const string& res : fres)
        cout << res << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    cin.ignore();
    string line;
    
    for (int i = 0; i < n + m; i++)
    {
        getline(cin, line);
        if (line.empty()) continue;
        istringstream iss(line);
        vector<string> paras;
        string word;
        while (iss >> word) paras.push_back(word);
        
        if (paras[0] == "echo") echo(paras);
        if (paras[0] == "mkdir") mkdir(paras);
        if (paras[0] == "rm") rm(paras);
        if (paras[0] == "mv") mv(paras);
        if (paras[0] == "cat") cat(paras);
        if (paras[0] == "find") find(paras);
    }
    
    return 0;
}
