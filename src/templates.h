#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <cmath>
#include "../lib/tgaimage.h"

// TODO: реализация стандартной структуры для квадратных матриц(максимум нужно 4х4) со следующими операторами и методами: умножение, сложение, инверсия.

/// @brief
/// @tparam t
template <typename T, int N>
struct Coordinates
{
    union // Назначаются разные алиасы для одних переменных для более удобного использования в разных случаях
    {
        struct
        {
            T x, y, z, w;
        };
        struct
        {
            T R, G, B, A;
        };
        struct
        {
            T ivert, iuv, inorm, w;
        };
        struct
        {
            T u, v, z, w;
        };
        T raw[N];
    };

    Coordinates()
    {
        for (int i = 0; i < N; i++)
            raw[i] = 0;
    }

    template <typename... Args>
    Coordinates(Args... args) : raw{static_cast<T>(args)...}
    {
        static_assert(sizeof...(Args) <= N, "Too many arguments for Coordinates<N>");
        for (int i = sizeof...(Args); i < N; ++i)
            raw[i] = 0;
    }

    inline Coordinates<T, N> operator^(const Coordinates<T, N> &other) const
    {
        if constexpr (N >= 3)
            return Coordinates<T, N>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        else
            return x * other.y - y * other.x;
    }

    inline Coordinates<T, N> operator+(const Coordinates<T, N> &other) const
    {
        if constexpr (N >= 3)
            return Coordinates<T, N>(x + other.x, y + other.y, z + other.z);
        else
            return Coordinates<T, N>(x + other.x, y + other.y);
    }
    inline Coordinates<T, N> operator-(const Coordinates<T, N> &other) const
    {
        if constexpr (N >= 3)
            return Coordinates<T, N>(x - other.x, y - other.y, z - other.z);
        else
            return Coordinates<T, N>(x - other.x, y - other.y);
    }

    inline Coordinates<T, N> operator*(T f) const
    {
        if constexpr (N >= 3)
            return Coordinates<T, N>(x * f, y * f, z * f);
        else
            return Coordinates<T, N>(x * f, y * f);
    }

    inline T operator*(const Coordinates<T, N> &other) const // Тут скалярное произведение оно же dot product
    { 
        if constexpr (N >= 3)
            return x * other.x + y * other.y + z * other.z;
        else
            return x * other.x + y * other.y;
    }

    inline T operator[](int i) const {
        return raw[i];
    }

    bool operator==(const Coordinates<T, N>& other) const noexcept {
        return x == other.x && y == other.y;
    }

    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Coordinates<T, N> &normalize(T l = 1)
    {
        *this = (*this) * (l / norm());
        return *this;
    }
};



typedef Coordinates<float, 4> Coordinates4f;
typedef Coordinates<int, 4> Coordinates4i;

typedef Coordinates<float, 3> Coordinates3f;
typedef Coordinates<double, 3> Coordinates3d;
typedef Coordinates<int, 3> Coordinates3i;

typedef Coordinates<float, 2> Coordinates2f;
typedef Coordinates<int, 2> Coordinates2i;

#endif // TEMPLATES_H