#include "matrix.h"
#include <algorithm>

using namespace std;

Matrix::Matrix(const int width, const int height, const int default_value) : 
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

int& Matrix::operator[] (const Vectori& vector) {
    return values_[vector.y * height_ + vector.x];
}

int Matrix::operator[] (const Vectori& vector) const {
    return values_[vector.y * height_ + vector.x];
}

int Matrix::GetValue(const int x, const int y) const {
    return values_[y * height_ + x];
}

int Matrix::GetWidth() const {
    return width_;
}

int Matrix::GetHeight() const {
    return height_;
}

int Matrix::GetMinValue() const { 
    return *min_element(values_.begin(), values_.end());
}

int Matrix::GetMaxValue() const { 
    return *max_element(values_.begin(), values_.end());
}

std::ostream &operator<< (std::ostream &os, const Matrix &m) {
    for (auto y = m.GetHeight() - 1; y >= 0; y--) {
        for (auto x = 0; x < m.GetWidth(); x++) {
	        const auto value = m.GetValue(x, y);
            if ( value == -1) 
                os << " ";
            else
                os << value;
        }
        os << std::endl;
    }
  return os;
};