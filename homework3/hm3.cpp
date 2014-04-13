/*
 * Author:                  Jonathan McElroy
 * Assignment Title:        Homework 3
 * Assignment Description:  Finds the closest pair of points
 * Due Date:                2/6/14
 * Date Created:            2/6/14
 * Date Late Modified:      2/6/14
 */
#include <iostream>
using namespace std;

int main() {
    int num;
    cin >> num;
    float* x = new float[num];
    float* y = new float[num];
    int min_num1, min_num2;
    float min_dist=800000, testx, testy, xdiff, ydiff, test_dist;
    for(int i=0; i<num; i++) {
        cin >> testx >> testy;
        for(int j=0; j<i; j++) {
            xdiff = testx - x[j];
            ydiff = testy - y[j];
            test_dist = xdiff*xdiff + ydiff*ydiff;
            if (test_dist < min_dist) {
                min_dist = test_dist;
                min_num1 = j;
                min_num2 = i;
            }
        }
        x[i] = testx;
        y[i] = testy;
    }
    cout << min_num1 << ' ' << min_num2 << endl;
    delete [] x;
    delete [] y;

    return 0;
}
