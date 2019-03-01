#include "vectori.h"
#include "vectord.h"

using namespace std;

Vectori::Vectori() : Vectori(0, 0) {
}

Vectori::Vectori(const int x, const int y) : x(x), y(y) {    
}

Vectori::Vectori(const Vectord& vector) : x(vector.x), y(vector.y) {    
}

Vectori Vectori::operator+(const Vectori &v) const {
    return Vectori(x+v.x, y+v.y);
}

Vectori Vectori::operator-(const Vectori &v) const {
    return Vectori(x-v.x, y-v.y);
}

Vectord Vectori::operator+(const Vectord &v) const {
    return Vectord(x+v.x, y+v.y);
}

Vectord Vectori::operator-(const Vectord &v) const {
    return Vectord(x-v.x, y-v.y);
}

bool Vectori::operator==(const Vectori &v) const {
    return (x == v.x && y == v.y);
}

bool Vectori::operator!=(const Vectori &v) const {
    return (x != v.x || y != v.y);
}

Vectori Vectori::operator-() const {
    return Vectori(-x, -y);
}

Vectord Vectori::operator*(const float& scalar) const {
    return Vectord(x*scalar, y*scalar);
}

Vectord Vectori::operator/(const float& scalar) const {
    return Vectord(x/scalar, y/scalar);
}

std::ostream &operator<< (std::ostream &os, const Vectori &v) {
  os << "(" << v.x << ", " << v.y << ")";
  return os;
};