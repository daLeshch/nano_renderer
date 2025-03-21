#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include "tgaimage.h"

/// @brief 
/// @tparam t 
template <class t>
struct Coordinates3 {
    union {
            struct {t x, y, z;};
            struct {t ivert, iuv, inorm;};
            t raw[3];
    };

    Coordinates3(): x(0), y(0), z(0) {}
    Coordinates3(t _x, t _y, t _z): x(_x), y(_y), z(_z) {}

    inline Coordinates3<t> operator ^(const Coordinates3<t> &v) const { return Coordinates3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Coordinates3<t> operator +(const Coordinates3<t> &v) const { return Coordinates3<t>(x+v.x, y+v.y, z+v.z); }
	inline Coordinates3<t> operator -(const Coordinates3<t> &v) const { return Coordinates3<t>(x-v.x, y-v.y, z-v.z); }
	inline Coordinates3<t> operator *(float f)          const { return Coordinates3<t>(x*f, y*f, z*f); }
    inline t       operator *(const Coordinates3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
    float norm() const { return std::sqrt(x*x+y*y+z*z); }
    Coordinates3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
};


typedef Coordinates3<float> Coordinates3f;
typedef Coordinates3<int>   Coordinates3i;

#endif // GEOMETRY_H