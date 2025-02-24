#include <iostream>
using namespace std;

struct Node {
    char ch;
    Node* left;
    Node* right;

    Node(char val) : ch(val), left(nullptr), right(nullptr) {}
};

void ino(Node* root)
{
    if (root == nullptr) return;
    if (!(root->ch >= 'a' and root->ch <= 'z')) cout << "(";
    ino(root->left);
    cout << root->ch;
    ino(root->right);
    if (!(root->ch >= 'a' and root->ch <= 'z')) cout << ")";
}

void preo(Node* root)
{
    if (root == nullptr) return;
    cout << root->ch;
    preo(root->left);
    preo(root->right);
}

void posto(Node* root)
{
    if (root == nullptr) return;
    posto(root->left);
    posto(root->right);
    cout << root->ch;
}

Node* build(const char* pre)
{
    int len = 0;
    while (pre[len] != '\0') len ++;

    Node* stack[len];
    int top = -1;

    for (int i = len-1; i>=0; i--)
    {
        char c = pre[i];
        if (c == ' ' or c == ',' or c == '(' or c == ')') continue;
        if (c >= 'a' and c <= 'z') {
            Node* node = new Node(c);
            stack[++ top] = node;
        } else {
            Node* node = new Node(c);
            Node* l = stack[top --];
            Node* r = stack[top --];

            node->left = l;
            node->right = r;
            stack[++ top] = node;
        }
    }
    return stack[top];
}

int main()
{
    char pre[20001];
    cin.getline(pre, 20001);

    Node* root = build(pre);
    ino(root);
    cout << endl;
    preo(root);
    cout << endl;
    posto(root);
    cout << endl;

    return 0;
}
