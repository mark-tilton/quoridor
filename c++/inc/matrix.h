#ifndef QUORIDORBOT_MATRIX_H_
#define QUORIDORBOT_MATRIX_H_

#include <ostream>
#include <vector>
#include <sstream>
#include <iterator>
#include "vectori.h"

class Matrix {
public:
    Matrix(int width = 9, int height = 9, int default_value = 0);
    Matrix(const Matrix& other);
    ~Matrix();

    int& operator[] (const Vectori&);
    int operator[] (const Vectori&) const;
    int GetValue(int x, int y) const;
    int GetWidth() const;
    int GetHeight() const;
    int GetMinValue() const;
    int GetMaxValue() const;

    template <typename Writer>
    void Serialize(Writer& writer) const {
        std::stringstream result;
        std::copy(values_.begin(), values_.end(), std::ostream_iterator<int>(result, ""));
        writer.String(result.str().c_str());
    }

private:
int width_;
int height_;
std::vector<int> values_;

};

std::ostream &operator<< (std::ostream &os, const Matrix &m);

#endif