#include <vector>
#include <iostream>
#include <cassert>
#include "Matrix.h"
using namespace std;

Matrix::Matrix() {
    this->data = vector<vector<int> >();
}

Matrix::Matrix(const Matrix& m) {
    this->data = m.data;
}

Matrix::Matrix(int size) {
    this->data = vector<vector<int> >(size);
    for(int row=0; row < size; row++) {
        this->data[row].resize(size, 0);
    }
}

Matrix::Matrix(vector<vector<int> > data) {
    this->data = data;
}

int Matrix::get(int row, int col) const {
    assert(row < this->getSize() && col < this->getSize());
    return this->data[row][col];
}

void Matrix::set(int row, int col, int value) {
    assert(row < this->getSize() && col < this->getSize());
    this->data[row][col] = value;
}

int Matrix::getSize() const {
    return this->data.size();
    //return 2;
}

vector<Matrix> Matrix::split(){
    assert(this->getSize() > 1 && this->getSize()%2 == 0);
    vector<Matrix> matrices;
    for(int row=0; row < this->getSize()/2; row++) {
        cout << row << ": ";
        for(int col=0; col < this->getSize()/2; col++) {
            cout << col << " ";
            matrices[NWINDEX].set(row, col, this->data[row][col]);
        }
        cout << endl;
    }
    cout << "Here!" << endl;
    for(int row=this->getSize()/2; row < this->getSize(); row++) {
        for(int col=0; col < this->getSize()/2; col++) {
            matrices[SWINDEX].set(row, col, this->data[row+this->getSize()/2][col]);
        }
    }
    for(int row=0; row < this->getSize()/2; row++) {
        for(int col=0; col < this->getSize()/2; col++) {
            matrices[NEINDEX].set(row, col, this->data[row][col+this->getSize()/2]);
        }
    }
    for(int row=this->getSize()/2; row < this->getSize(); row++) {
        for(int col=0; col < this->getSize()/2; col++) {
            matrices[SEINDEX].set(row, col, this->data[row+this->getSize()/2][col+this->getSize()/2]);
        }
    }
    return matrices;
}
Matrix Matrix::combine(const Matrix mNW, const Matrix mNE, const Matrix mSW, const Matrix mSE) {
    int size = mNW.getSize();
    assert(size == mNE.getSize());
    assert(size == mSW.getSize());
    assert(size == mSE.getSize());

    Matrix result(size*2);
    for(int row=0; row < size; row++) {
        for(int col=0; col < size; col++) {
            result.set(row, col, mNW.data[row][col]);
        }
    }
    for(int row=0; row < size; row++) {
        for(int col=0; col < size; col++) {
            result.set(row, col+size, mNE.data[row][col]);
        }
    }
    for(int row=0; row < size; row++) {
        for(int col=0; col < size; col++) {
            result.set(row+size, col, mSW.data[row][col]);
        }
    }
    for(int row=0; row < size; row++) {
        for(int col=0; col < size; col++) {
            result.set(row+size, col+size, mSE.data[row][col]);
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& m) const {
    Matrix result(this->getSize());
    for(int i=0; i < this->getSize(); i++) {
        for(int j=0; j < this->getSize(); j++) {
            result.set(i, j, this->get(i,j) + m.get(i,j));
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& m) const {
    Matrix result(this->getSize());
    for(int i=0; i < this->getSize(); i++) {
        for(int j=0; j < this->getSize(); j++) {
            result.set(i, j, this->get(i,j) - m.get(i,j));
        }
    }
    return result;
}

ostream& operator<<(ostream& out, const Matrix& m) {
    //return out << "Here!" << endl;
    //return out << m.data << endl;
}
