#include <iostream>
#include "maxValue.h"
using namespace std;

int main() {
    int a[5] = {1,3,5,2,-4};
    int size = 5;
    cout << "The max value of 1, 3, 5, 2, and -5 is: " << maxValue(a, size) << endl;

    char ch[17] = "Jonathan McElroy";
    size = 17;
    cout << "The max value of the letters in \"Jonathan McElroy\" is: " << maxValue(ch, size) << endl;

    short sh[10] = {1,1,1,1,1,1,1,1,1,1};
    size = 10;
    cout << "The max value of a series of 10 1's is: " << maxValue(sh, size) << endl;

    return 0;
}
