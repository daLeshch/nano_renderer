#define _USE_MATH_DEFINES

#ifndef MATH_CORE_H
#define MATH_CORE_H

#include <optional>
#include <array>
#include <initializer_list>
#include <cmath>
#include "tgaimage.h"

#define M_PI 3.14159265358979323846
/// @brief
/// @tparam t
template <typename T, int N>
struct vec // Структура для работы с 2D/3D/4D векторами
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

    vec()
    {
        for (int i = 0; i < N; i++)
            raw[i] = 1;
    }

    template <typename... Args>
    vec(Args... args) : raw{static_cast<T>(args)...}
    {
        static_assert(sizeof...(Args) <= N, "Too many arguments for vec<N>");
        for (int i = sizeof...(Args); i < N; ++i)
            raw[i] = 1;

        if (N == 3)
        {
            w = 1;
        }
        else if (N == 2)
        {
            z = std::nan("");
            w = std::nan("");
        }
    }

    inline vec<T, N> operator^(const vec<T, N> &other) const // cross тут, для 2D -- псевдокросс
    {
        if constexpr (N >= 3)
            return vec<T, N>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        else
            return x * other.y - y * other.x;
    }

    inline vec<T, N> operator+(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x + other.x, y + other.y, z + other.z);
        else
            return vec<T, N>(x + other.x, y + other.y);
    }
    inline vec<T, N> operator-(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x - other.x, y - other.y, z - other.z);
        else
            return vec<T, N>(x - other.x, y - other.y);
    }

    inline vec<T, N> operator*(T f) const // Просто умножение на число
    {
        if constexpr (N >= 3)
            return vec<T, N>(x * f, y * f, z * f);
        else
            return vec<T, N>(x * f, y * f);
    }
    inline vec<T, N> operator/(T f) const // Просто деление на число
    {
        if constexpr (N >= 3)
            return vec<T, N>(x / f, y / f, z / f);
        else
            return vec<T, N>(x / f, y / f);
    }

    inline T operator*(const vec<T, N> &other) const // А тут скалярное произведение оно же dot product
    {
        if constexpr (N >= 3)
            return x * other.x + y * other.y + z * other.z;
        else
            return x * other.x + y * other.y;
    }

    inline const T &operator[](int i) const
    {
        return raw[i];
    }

    inline T &operator[](int i) { return raw[i]; }

    bool operator==(const vec<T, N> &other) const noexcept
    {
        return std::equal(begin(), end(), other.begin(), other.end());
    }

    float norm() const
    {
        float sum = 0;
        for (int i = 0; i < N; ++i)
            sum += raw[i] * raw[i];
        return std::sqrt(sum);
    }

    vec<T, N> &normalize(T l = 1)
    {
        if (norm() == 0)
            throw std::runtime_error("Division by zero");
        *this = (*this) * (l / norm());
        return *this;
    }

    constexpr T *begin() { return raw; }
    constexpr T *end() { return raw + N; }

    constexpr const T *begin() const { return raw; }
    constexpr const T *end() const { return raw + N; }
};

typedef vec<float, 4> vec4f;
typedef vec<int, 4> vec4i;
typedef vec<double, 4> vec4d;

typedef vec<float, 3> vec3f;
typedef vec<double, 3> vec3d;
typedef vec<int, 3> vec3i;

typedef vec<float, 2> vec2f;
typedef vec<int, 2> vec2i;

template <size_t rows, size_t cols, typename T = double>
struct Matrix
{
    std::array<std::array<T, cols>, rows> data{};

    constexpr Matrix() = default;
    constexpr Matrix(std::initializer_list<std::initializer_list<T>> values)
    {
        size_t i = 0;
        for (auto &row : values)
        {
            size_t j = 0;
            for (auto &val : row)
            {
                if (i < rows && j < cols)
                    data[i][j] = val;
                ++j;
            }
            ++i;
        }
    };

    constexpr auto operator[](size_t row) -> std::array<T, cols> &
    {
        return data[row];
    }

    constexpr auto operator[](size_t row) const -> const std::array<T, cols> &
    {
        return data[row];
    };

    // Операторы модифицирующие data

    // Скалярные:
    constexpr Matrix &operator+=(const T &scalar)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = (*this)[i][j] + scalar;
            }
        }
        return *this;
    };
    constexpr Matrix &operator-=(const T &scalar)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = (*this)[i][j] - scalar;
            }
        }
        return *this;
    };
    constexpr Matrix &operator*=(const T &scalar)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = (*this)[i][j] * scalar;
            }
        }
        return *this;
    };

    // Матричные:

    constexpr Matrix<rows, cols, T> operator+=(const Matrix<rows, cols, T> &other)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = (*this)[i][j] + other[i][j];
            }
        }
        return *this;
    };

    constexpr Matrix<rows, cols, T> operator-=(const Matrix<rows, cols, T> &other)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                data[i][j] = (*this)[i][j] - other[i][j];
            }
        }
        return *this;
    };

    template <size_t otherCols>
    // constexpr Matrix<rows, otherCols, T> operator*=(const Matrix<cols, otherCols, T> &rhs) {
    //     for (size_t i=0; i<rows; ++i){
    //         for (size_t j=0; j<otherCols;++j){
    //             for (size_t k=0;k<cols;++k){
    //                 data[i][j] += (*this)[i][k] * rhs[k][j];
    //             }
    //         }
    //     }
    // return *this;
    // };

    // Операторы для вычислений, новые объекты
    // Скалярные
    constexpr Matrix operator+(const T &scalar) const
    {
        Matrix<rows, cols, T> res{};
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                res[i][j] = (*this)[i][j] + scalar;
            }
        }
        return res;
    };
    constexpr Matrix<rows, cols, T> operator-(const T &scalar) const
    {
        Matrix<rows, cols, T> res{};

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                res[i][j] = (*this)[i][j] - scalar;
            }
        }
        return res;
    };
    constexpr Matrix<rows, cols, T> operator*(const T &scalar) const
    {
        Matrix<rows, cols, T> res{};

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                res[i][j] = (*this)[i][j] * scalar;
            }
        }
        return res;
    };

    constexpr Matrix operator/(const T &scalar) const
    {
        if (scalar == 0)
            throw std::runtime_error("Division by zero");
        Matrix res{};
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                res[i][j] = (*this)[i][j] / scalar;
        return res;
    };

    // Матричные
    constexpr Matrix<rows, cols, T> operator+(const Matrix<rows, cols, T> &other) const
    {
        Matrix<rows, cols, T> res{};

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                res[i][j] = (*this)[i][j] + other[i][j];
            }
        }
        return res;
    };

    constexpr Matrix<rows, cols, T> operator-(const Matrix<rows, cols, T> &other) const
    {
        Matrix<rows, cols, T> res{};

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                res[i][j] = (*this)[i][j] - other[i][j];
            }
        }
        return res;
    };

    template <size_t otherCols>
    constexpr Matrix<rows, otherCols, T> operator*(const Matrix<cols, otherCols, T> &rhs) const
    {
        Matrix<rows, otherCols, T> res{};

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < otherCols; ++j)
            {
                for (size_t k = 0; k < cols; ++k)
                {
                    res[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return res;
    };

    // Векторное
    constexpr vec<T, rows> operator*(const vec<T, cols> &vect) const
    {
        vec<T, rows> fin{};
        for (size_t i = 0; i < rows; ++i)
        {
            T res = 0;
            for (size_t j = 0; j < cols; ++j)
            {
                res += (*this)[i][j] * vect[j];
            }
            fin[i] = res;
        }
        return fin;
    };

    // Методы для всяких трансформаций
    double det()
    {
        static_assert(rows == cols, "Must be a square matrix!");
        Matrix<rows, cols, T> mat = *this;

        if (rows == 2)
            return (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]); // исключаем случай матрицы 2х2

        double det = 1;
        T sign = 1;

        for (size_t i = 0; i < rows; ++i)
        { // тут индекс пивота. [i] - просто строки, [i][i] - сам пивот
            if (mat[i][i] == 0)
            {
                bool pivot = false;
                for (size_t c = i + 1; c < rows; ++c)
                {
                    if (mat[c][i] != 0)
                    {
                        std::swap(mat[i], mat[c]);
                        pivot = true;
                        sign *= -1;
                        break;
                    }
                }
                if (!pivot)
                    return 0;
            }
            for (size_t j = i + 1; j < rows; ++j)
            { // тут индекс строки под пивотом [j] - просто строки ниже, [j][i] -  точка под пивотом

                double coef = mat[j][i] / mat[i][i];

                for (size_t k = i; k < rows; k++)
                { // а вот тут уже обход в сторону
                    mat[j][k] -= mat[i][k] * coef;
                }
            }
        }

        for (size_t i = 0; i < rows; ++i)
        {
            det *= mat[i][i];
        }
        return det * sign;
    }

    Matrix<cols, rows, T> transpose() const
    {
        Matrix<cols, rows, T> trans;
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                trans[j][i] = (*this)[i][j];
            }
        }
        return trans;
    }

    std::optional<Matrix<rows, cols, T>> inverse() const
    {
        static_assert(rows == cols, "Must be a square matrix!");
        Matrix<rows, cols, T> mat = *this;
        Matrix<rows, cols, T> inv{};
        for (size_t i = 0; i < rows; ++i)
            inv[i][i] = 1;
        // прямой проход
        for (size_t i = 0; i < rows; ++i)
        { // тут индекс пивота. [i] - просто строки, [i][i] - сам пивот
            if (mat[i][i] == 0)
            {
                bool pivot = false;
                for (size_t c = i + 1; c < rows; ++c)
                {
                    if (mat[c][i] != 0)
                    {
                        std::swap(mat[i], mat[c]);
                        std::swap(inv[i], inv[c]);
                        pivot = true;
                        break;
                    }
                }
                if (!pivot)
                    return std::nullopt;
            }
            for (size_t k = 0; k < rows; k++)
            { // нормализация диагонали
                mat[i][k] = mat[i][k] / mat[i][i];
                inv[i][k] = inv[i][k] / mat[i][i];
            }
            for (size_t j = i + 1; j < rows; ++j)
            { // тут индекс строки под пивотом [j] - просто строки ниже, [j][i] -  точка под пивотом

                double coef = mat[j][i] / mat[i][i];

                for (size_t k = i; k < rows; k++)
                { // а вот тут уже обход в сторону
                    mat[j][k] -= mat[i][k] * coef;
                    inv[j][k] -= inv[i][k] * coef;
                }
            }
        }
        // обратный проход
        for (int i = rows - 1; i >= 0; --i)
        { // тут индекс пивота. [i] - просто строки, [i][i] - сам пивот
            for (int j = i - 1; j >= 0; --j)
            { // тут индекс строки под пивотом [j] - просто строки ниже, [j][i] -  точка под пивотом
                double coef = mat[j][i] / mat[i][i];

                for (size_t k = 0; k < rows; k++)
                { // а вот тут уже обход в сторону
                    mat[j][k] -= mat[i][k] * coef;
                    inv[j][k] -= inv[i][k] * coef;
                }
            }
        }
        return inv;
    }
};

typedef Matrix<3, 3> mat3;
typedef Matrix<4, 4, float> mat4;

#endif // MATH_CORE_H
