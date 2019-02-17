#include "matrix.h"

Matrix::Matrix(int width, int height) : width_(width), height_(height) {
    values_ = new int[width * height]();
}

Matrix::~Matrix() {
    delete[] values_;
}

int& Matrix::operator[] (Vectori vector) {
    return values_[vector.y * height_ + vector.x];
}

int& Matrix::operator[] (Vectori vector) const {
    return values_[vector.y * height_ + vector.x];
}

void Matrix::SetValues(int value) {
    for (int i = 0; i < width_ * height_; i++) {
        values_[i] = value;
    }
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
    for (int y = 0; y < m.GetHeight(); y++) {
        for (int x = 0; x < m.GetWidth(); x++) {
            os << m.GetValue(x, y);
        }
        os << std::endl;
    }
  return os;
};