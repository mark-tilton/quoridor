#include "vectord.h"
#include "vectori.h"

using namespace std;

Vectord::Vectord() : Vectord(0, 0) {
}

Vectord::Vectord(const double x, const double y) : x(x), y(y) {    
}

Vectord::Vectord(const Vectori& vector) : x(vector.x), y(vector.y) {    
}

Vectord Vectord::operator+(const Vectord &v) const {
    return Vectord(x+v.x, y+v.y);
}

Vectord Vectord::operator-(const Vectord &v) const {
    return Vectord(x-v.x, y-v.y);
}

bool Vectord::operator==(const Vectord &v) const {
    return (x == v.x && y == v.y);
}

bool Vectord::operator!=(const Vectord &v) const {
    return (x != v.x || y != v.y);
}

Vectord Vectord::operator-() const {
    return Vectord(-x, -y);
}

Vectord Vectord::operator*(const float& scalar) const {
    return Vectord(x*scalar, y*scalar);
}

Vectord Vectord::operator/(const float& scalar) const {
    return Vectord(x/scalar, y/scalar);
}

std::ostream &operator<< (std::ostream &os, const Vectord &v) {
  os << "(" << v.x << ", " << v.y << ")";
  return os;
};