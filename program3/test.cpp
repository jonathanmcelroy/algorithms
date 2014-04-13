#include <iostream>
#include <algorithm>
using namespace std;

int _main() {
    int size = 10;
    int L[size];
    for(int i=0; i<size; i++) {
        L[i] = i;
        cout << i << endl;
    }
    cout << endl;

    cout << "Found 5 at index: " << *find(L, L+size, 5) << endl;

    return 0;
}
