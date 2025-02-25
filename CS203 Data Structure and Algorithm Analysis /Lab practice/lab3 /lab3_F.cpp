#include <iostream>
#include <vector>
using namespace std;

class Node {
public:
    int value;
    Node* next;
    Node* last;
    int index;
    bool b;

    Node(int value) : value(value), next(nullptr), last(nullptr), index(0), b(true) {}
};

int main()
{
    int T;
    cin >> T;
    
    for (int i=0; i<T; i++)
    {
        int n;
        Node* head = new Node(0);
        head->index = 0;
        Node* cur = head;
        cin >> n;
        int ind = 1;

        for (int j=0; j<n; j++)
        {
            int x;
            cin >> x;
            cur->next = new Node(x);
            cur->next->index = ind++;
            cur->next->last = cur;
            cur = cur->next;
        }

        Node* tail = new Node(0);
        tail->index = 0;
        cur->next = tail;
        tail->last = cur;
        cur = head->next;
        vector<Node*> check(n);
        int num = 0;

        while (cur->value != 0 && cur->next->value != 0) {
            if (cur->next->value >= cur->value) {
                cur = cur->next;
            } else {
                Node* end = cur->next;
                cur->b = false;
                end->b = false;

                while (end->next->value != 0 and end->next->value < end->value) {
                    end = end->next;
                    end->b = false;
                }

                cur->last->next = end->next;
                end->next->last = cur->last;

                if (cur->last->value != 0)
                {
                    if (num >= 1)
                    {
                        if (check[num - 1]->index != cur->last->index) {
                            check[num++] = cur->last;
                        }
                    } else {
                        check[num++] = cur->last;
                    }
                }
                cur = end->next;
            }
        }

        while (num != 0)
        {
            int number = num;
            num = 0;
            for (int j=0; j<number; j++)
            {
                if (!check[j]->b) continue;
                cur = check[j];
                if (cur->next->value == 0) break;
                if (cur->next->value >= cur->value) continue;
                else {
                    Node* end = cur->next;
                    cur->b = false;
                    end->b = false;

                    while (end->next->value != 0 && end->next->value < end->value) {
                        end = end->next;
                        end->b = false;
                    }

                    cur->last->next = end->next;
                    end->next->last = cur->last;

                    if (cur->last->value != 0)
                    {
                        if (num >= 1)
                        {
                            if (check[num - 1]->index != cur->last->index) {
                                check[num++] = cur->last;
                            }
                        } else {
                            check[num++] = cur->last;
                        }
                    }
                }
            }
        }

        cur = head->next;
        while (cur->value != 0) {
            cout<<cur->value<<" ";
            cur = cur->next;
        }
        cout<<endl;
    }
    return 0;
}
