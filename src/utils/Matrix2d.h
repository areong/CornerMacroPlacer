#ifndef UTILS_MATRIX2D_H_
#define UTILS_MATRIX2D_H_

#include <vector>

class Matrix2d {
public:
    Matrix2d(int numRows, int numCols, double value=0);
    ~Matrix2d();
    int getNumRows();
    int getNumCols();
    void setValue(int i, int j, double value);
    void setAllValues(double value);
    double getValue(int i, int j);
    void multiplyAt(int i, int j, double value);
    /*
    Add elements of matrix2 to this Matrix2d.
    Assume matrix2 with offset is inside this Matrix2d.
    */
    void add(Matrix2d *matrix2, int offsetI, int offsetJ);
    /*
    Return the dot product of matrix2 and this Matrix2d.
    matrix2 with offset can extend outside this Matrix2d's range.
    */
    double dot(Matrix2d *matrix2, int offsetI, int offsetJ);

    void print();

private:
    int numRows;
    int numCols;
    std::vector<std::vector<double> *> *elements;
};

#endif