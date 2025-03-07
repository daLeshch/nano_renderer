#include <cmath>
#include "tgaimage.h"

/// @brief 
/// @tparam T 
template <typename T> struct Coordinates3<T> {
    union {
            struct {T x, y, z;};
            struct {T ivert, iuv, inorm;};
            T raw[3];
    };

    Coordinates3(): x(0), y(0), z(0) {}
    Coordinates3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}

    inline Coordinates3<T> operator ^(const Coordinates3<T> &v) const { return Coordinates3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);}
    inline Coordinates3<T> operator +(const Coordinates3<T> &v) const { return Coordinates3<T>(x + v.x, y + v.y, z + v.z);}
    inline Coordinates3<T> operator -(const Coordinates3<T> &v) const { return Coordinates3<T>(x - v.x, y - v.y, z - v.z);}
    inline Coordinates3<T> operator *(float f) const { return Coordinates3<T>(x * f, y * f, z * f);}
    inline T operator *(const Coordinates3<T> &v) const { return Coordinates3<T>(x * v.x, y * v.y, z * v.z);}
    float norm() const { return std::sqrt(x*x+y*y+z*z); }
    Coordinates3<T> & normalize(T l = 1) { *this = (*this)*(1/norm());}
};

/// @brief 
/// @tparam T 
template <typename T> struct Coordinates2<T> {
    union {
            struct {T x, y;};
            struct {T iu, iv;};
            T raw[2];
    };

    Coordinates2(): x(0), y(0) {}
    Coordinates2(T _x, T _y): x(_x), y(_y) {}

    inline Coordinates2<T> operator +(const Coordinates2<T> &v) const { return Coordinates2<T>(x + v.x, y + v.y);}
    inline Coordinates2<T> operator -(const Coordinates2<T> &v) const { return Coordinates2<T>(x - v.x, y - v.y);}
    inline Coordinates2<T> operator *(float f) const { return Coordinates2<T>(x * f, y * f);}
};

typedef Coordinates3<float> Coordinates3f;
typedef Coordinates3<int>   Coordinates3i;
typedef Coordinates2<float> Coordinates2f;
typedef Coordinates2<int>   Coordinates2i;