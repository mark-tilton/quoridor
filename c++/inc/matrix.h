#ifndef QUORIDORBOT_MATRIX_H_
#define QUORIDORBOT_MATRIX_H_

#include <ostream>
#include "vectori.h"

class Matrix {
public:
    Matrix(int width, int height);
    ~Matrix();

    int& operator[] (Vectori); 
    int& operator[] (Vectori) const; 
    void SetValues(int value); 
    int GetValue(int x, int y) const; 
    int GetValue(const Vectori& vector) const; 
    int GetWidth() const;
    int GetHeight() const;

private:
int* values_;
int width_;
int height_;

};

std::ostream &operator<< (std::ostream &os, const Matrix &m);

#endif