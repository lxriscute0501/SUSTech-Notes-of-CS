#include <iostream>
using namespace std;

int main()
{
   int n, t;
   int a[1001], b[1001];
   bool bo;
   
   cin >> n;
   for (int i=1; i<=n; i++)
       cin >> a[i];
   cin >> t;
   for (int i=1; i<=t; i++)
       cin >> b[i];
   for (int i=1; i<=t; i++)
   {
       bo = false;
       for (int j=1; j<=n; j++)
           if (b[i] == a[j]) {
               bo = true;
               break;
           }
       if (bo) cout << "yes" << endl; else cout << "no" << endl;
   }
}
