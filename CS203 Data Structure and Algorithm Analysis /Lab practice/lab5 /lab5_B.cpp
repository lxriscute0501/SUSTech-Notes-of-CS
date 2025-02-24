#include <iostream>
#include <algorithm>
#include <stdio.h>
using namespace std;

int main()
{
    char st[1000001];
    scanf("%s", st);
    
    int n = 0;
    while (st[n] != '\0') n ++;
  
    int i = 1, len = 0, next[1000001];
    while (i < n)
    {
        if (st[len] == st[i]) {
            next[i] = ++len;
            i++;
        } else {
            if (len == 0) {
                next[i] = len;
                i++;
            } else len = next[len - 1];
        }
    }

    for (int i = 0; i < n; i++)
        printf("%d\n", next[i]);
    
    return 0;
}
