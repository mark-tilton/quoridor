#include "matrix.h"
#include <cstring>

using namespace std;

Matrix::Matrix(int width, int height, int default_value) : 
    width_(width), 
    height_(height),
    values_(vector<int>(width * height, default_value)) {
}

Matrix::Matrix(const Matrix& other) : 
    width_(other.width_), 
    height_(other.height_),
    values_(other.values_) {
}

Matrix::~Matrix() {
}

int& Matrix::operator[] (Vectori vector) {
    return values_[vector.y * height_ + vector.x];
}

int Matrix::operator[] (Vectori vector) const {
    return values_[vector.y * height_ + vector.x];
}

int Matrix::GetValue(int x, int y) const {
    return values_[y * height_ + x];
}

int Matrix::GetWidth() const {
    return width_;
}

int Matrix::GetHeight() const {
    return height_;
}

std::ostream &operator<< (std::ostream &os, const Matrix &m) {
    for (int y = m.GetHeight() - 1; y >= 0; y--) {
        for (int x = 0; x < m.GetWidth(); x++) {
            auto value = m.GetValue(x, y);
            if ( value == -1) 
                os << " ";
            else
                os << value;
        }
        os << std::endl;
    }
  return os;
};