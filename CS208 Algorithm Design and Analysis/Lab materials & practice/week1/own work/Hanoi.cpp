#include <iostream>
using namespace std;

void hanoi(int n, char from_rod, char to_rod, char aux_rod) {
    if (n == 1) {
        cout << "Move disk 1 from " << from_rod << " to " << to_rod << endl;
        return;
    }
    hanoi(n - 1, from_rod, aux_rod, to_rod);
    cout << "Move disk " << n << " from " << from_rod << " to " << to_rod << endl;
    hanoi(n - 1, aux_rod, to_rod, from_rod);
}

int main() 
{
    int n = 3;
    hanoi(n, 'A', 'C', 'B');
    return 0;
}
