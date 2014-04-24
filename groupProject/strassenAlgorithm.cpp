#include <iostream>
#include <cassert>
#include "Matrix.h"
using namespace std;

Matrix strassen(Matrix m1, Matrix m2) {
    assert(m1.getSize() == m2.getSize());
    //cout << "Called on " << m1 << " and " << m2 << endl;

    if (m1.getSize() == 1) {
        Matrix result(1);
        result.set(0,0, m1.get(0,0) * m2.get(0,0));
        return result;
    }

    // split m1 into 4 submatrixes
    vector<Matrix> m1parts = m1.split();
    Matrix m1NW = m1parts[NWINDEX];
    Matrix m1NE = m1parts[NEINDEX];
    Matrix m1SW = m1parts[SWINDEX];
    Matrix m1SE = m1parts[SEINDEX];

    // split m2 into 4 submatrixes
    vector<Matrix> m2parts = m2.split();
    Matrix m2NW = m2parts[NWINDEX];
    Matrix m2NE = m2parts[NEINDEX];
    Matrix m2SW = m2parts[SWINDEX];
    Matrix m2SE = m2parts[SEINDEX];

    Matrix temp1 = strassen(m1NW + m1SE, m2NW + m2SE);
    Matrix temp2 = strassen(m1SW + m1SE, m2NW);
    Matrix temp3 = strassen(m1NW, m2NE + m2SE);
    Matrix temp4 = strassen(m1SE, m2NW + m2SW);
    Matrix temp5 = strassen(m1NW + m1NE, m2SE);
    Matrix temp6 = strassen(m1SW - m1NW, m2NW + m2NE);
    Matrix temp7 = strassen(m1NE - m1SE, m2SW + m2SE);

    Matrix resultNW = temp1 + temp4 - temp5 + temp7;
    Matrix resultNE = temp3 + temp5;
    Matrix resultSW = temp2 + temp4;
    Matrix resultSE = temp1 - temp2 + temp3 + temp6;

    return Matrix().combine(resultNW, resultNE, resultSW, resultSE);
}
