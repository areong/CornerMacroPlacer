#include "utils/Matrix2d.h"
#include <iostream>

Matrix2d::Matrix2d(int numRows, int numCols, double value) {
    this->numRows = numRows;
    this->numCols = numCols;
    elements = new std::vector<std::vector<double> *>(numRows, 0);
    for (int i = 0; i < numRows; ++i) {
        elements->at(i) = new std::vector<double>(numCols, value);
    }
}

Matrix2d::~Matrix2d() {
    for (int i = 0; i < numRows; ++i) {
        delete elements->at(i);
    }
    delete elements;
}

int Matrix2d::getNumRows() {
    return numRows;
}

int Matrix2d::getNumCols() {
    return numCols;
}

void Matrix2d::setValue(int i, int j, double value) {
    elements->at(i)->at(j) = value;
}

double Matrix2d::getValue(int i, int j) {
    return elements->at(i)->at(j);
}

void Matrix2d::setAllValues(double value) {
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            elements->at(i)->at(j) = value;
        }
    }
}

void Matrix2d::multiplyAt(int i, int j, double value) {
    elements->at(i)->at(j) *= value;
}

void Matrix2d::add(Matrix2d *matrix2, int offsetI, int offsetJ) {
    for (int i = 0; i < matrix2->getNumRows(); ++i) {
        for (int j = 0; j < matrix2->getNumCols(); ++j) {
            elements->at(i + offsetI)->at(j + offsetJ) += matrix2->getValue(i, j);
        }
    }
}

double Matrix2d::dot(Matrix2d *matrix2, int offsetI, int offsetJ) {
    int numRows2 = matrix2->getNumRows();
    int numCols2 = matrix2->getNumCols();

    // Check whether the two Matrix2ds overlap.
    int offsetI_add_numRows2 = offsetI + numRows2;
    int offsetJ_add_numCols2 = offsetJ + numCols2;
    if (offsetI >= numRows || offsetI_add_numRows2 <= 0 ||
        offsetJ >= numCols || offsetJ_add_numCols2 <= 0) {
        return 0;
    }

    // Determine the range of overlap on matrix2.
    int iStart = (offsetI > 0) ? 0 : -1 * offsetI;
    int jStart = (offsetJ > 0) ? 0 : -1 * offsetJ;
    int iEnd = (offsetI_add_numRows2 <= numRows) ? numRows2 : numRows - offsetI;
    int jEnd = (offsetJ_add_numCols2 <= numCols) ? numCols2 : numCols - offsetJ;

    double dotProduct = 0;
    for (int i = iStart; i < iEnd; ++i) {
        for (int j = jStart; j < jEnd; ++j) {
            dotProduct += elements->at(i + offsetI)->at(j + offsetJ) * matrix2->getValue(i, j);
        }
    }
    return dotProduct;
}

void Matrix2d::print() {
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            std::cout << elements->at(i)->at(j) << " ";
        }
        std::cout << "\n";
    }
}