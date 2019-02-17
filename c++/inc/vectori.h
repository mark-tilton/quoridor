#ifndef QUORIDORBOT_VECTORI_H_
#define QUORIDORBOT_VECTORI_H_

#include <ostream>

struct Vectord;

struct Vectori {

public:
    int x, y;

    Vectori();
    Vectori(int x, int y);
    Vectori(const Vectord& vector);

    Vectori operator+(const Vectori&) const;
    Vectori operator-(const Vectori&) const;
    Vectord operator-(const Vectord&) const;
    Vectord operator*(const float&) const;
    Vectord operator/(const float&) const;
    Vectori operator-() const;
    bool operator==(const Vectori&) const;
    bool operator!=(const Vectori&) const;
};

std::ostream &operator<< (std::ostream &os, const Vectori &v);

#endif