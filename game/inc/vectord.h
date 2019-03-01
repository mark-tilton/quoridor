#ifndef QUORIDORBOT_VECTORD_H_
#define QUORIDORBOT_VECTORD_H_

#include <ostream>

struct Vectori;

struct Vectord {

public:
    double x, y;

    Vectord();
    Vectord(double x, double y);
    Vectord(const Vectori& vector);

    Vectord operator+(const Vectord&) const;
    Vectord operator-(const Vectord&) const;
    Vectord operator*(const float&) const;
    Vectord operator/(const float&) const;
    Vectord operator-() const;
    bool operator==(const Vectord&) const;
    bool operator!=(const Vectord&) const;
};

std::ostream &operator<< (std::ostream &os, const Vectord &v);

#endif