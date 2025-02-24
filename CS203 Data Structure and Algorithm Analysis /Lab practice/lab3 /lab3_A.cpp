#include <iostream>
using namespace std;

struct node {
    int num;
    int dir;
    node* next;
};

int main()
{
    int t;
    cin >> t;
    for (int i=1; i<=t; i++)
    {
        int n, k, count=0, loc=0;
        cin >> n >> k;
        
        if (n == 1) {
            cout<<1<<endl;
            continue;
        }
        
        if (k == 1) {
            for (int j=1; j<=n; j++)
                cout<<j<<" ";
            cout<<endl;
            continue;
        }
        
        node* head = new node{1, 2};
        node* cur = head;
        
        for (int j=2; j<=n-1; j++)
        {
            node* temp = new node{j, j+1};
            cur->next = temp;
            cur = cur->next;
        }
        
        node* temp = new node{n, 1};
        cur->next = temp;
        cur = cur->next;
        cur->next = head;
        
        cur = head;
        loc = 1;
        
        while (count < n)
        {
            if (loc >= k-1) {
                cout << cur->dir << " ";
                cur->dir = cur->next->dir;
                cur->next = cur->next->next;
                loc = 0;
                count ++;
            }
            loc ++;
            cur = cur->next;
        }
        cout<<endl;
    }
    
    return 0;
}
