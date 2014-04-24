#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
using namespace std;

const int NWINDEX = 0;
const int NEINDEX = 1;
const int SWINDEX = 2;
const int SEINDEX = 3;
const int MATRIXNUMBER = 4;

class Matrix {
    private:
        vector<vector<int> > data;

    public:
        Matrix();
        Matrix(const Matrix& m);
        Matrix(int size);
        Matrix(vector<vector<int> > data);

        int get(int row, int col) const;
        void set(int row, int col, int value);
        int getSize() const;

        vector<Matrix> split();
        static Matrix combine(const Matrix, const Matrix, const Matrix, const Matrix);

        Matrix operator+(const Matrix& m2) const;
        Matrix operator-(const Matrix& m2) const;

        friend ostream& operator<<(ostream&, const Matrix&);
};

#endif // define MATRIX_H
