#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    char S[100001];
    scanf("%s", S);
  
    int n = 0;
    while (S[n] != '\0') n ++;
    
    int tr[100001][26], x = 0;
    tr[0][int(S[0]) - int('a')] = 1;
  
    for (int i = 1; i < n; i++)
    {
        for (char a = 'a'; a <= 'z'; a++)
            if (S[i] == a) tr[i][int(a) - int('a')] = i + 1;
            else tr[i][int(a) - int('a')] = tr[x][int(a) - int('a')];
        x = tr[x][int(S[i]) - int('a')];
    }

    for (int i = 0; i < n; i++)
    {
        for (int c = 0; c < 26; c++)
            printf("%d ", tr[i][c]);
        printf("\n");
    }

    return 0;
}
