#ifndef QUORIDORBOT_MATRIX_H_
#define QUORIDORBOT_MATRIX_H_

#include <ostream>
#include <vector>
#include "vectori.h"

class Matrix {
public:
    Matrix(int width = 9, int height = 9, int default_value = 0);
    Matrix(const Matrix& other);
    ~Matrix();

    int& operator[] (Vectori);
    int operator[] (Vectori) const;
    int GetValue(int x, int y) const;
    int GetValue(const Vectori& vector) const;
    int GetWidth() const;
    int GetHeight() const;
    int GetMinValue() const;
    int GetMaxValue() const;

private:
int width_;
int height_;
std::vector<int> values_;

};

std::ostream &operator<< (std::ostream &os, const Matrix &m);

#endif