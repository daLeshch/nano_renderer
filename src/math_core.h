/// @file math_core.h
/// @brief Custom mathematical library for vector and matrix operations.
/*!
    This header defines generic mathematical structures and operations,
    including N-dimensional vectors (`vec<T, N>`) and general-purpose matrices (`Matrix<rows, cols, T>`).
    It is designed for lightweight computer graphics and linear algebra tasks.
 */
#define _USE_MATH_DEFINES

#ifndef MATH_CORE_H
#define MATH_CORE_H

#include <optional>
#include <array>
#include <initializer_list>
#include <cmath>
#include "tgaimage.h"

#define M_PI 3.14159265358979323846
/// @brief Generic N-dimensional vector template.
/*!
    This structure supports mathematical operations such as addition, subtraction,
    dot and cross products, normalization, and scalar operations.
 */
/// @tparam T Data type of elements (e.g., float, double, int).
/// @tparam N Number of elements in the vector.
template <typename T, int N>
struct vec
{
    /// @brief Union to allow multiple convenient access patterns.
    union
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
    /// @brief Default constructor. Initializes all elements to 1.
    vec()
    {
        for (int i = 0; i < N; i++)
            raw[i] = 1;
    }
    /// @brief Variadic constructor for flexible initialization.
    /*!
        Fills up to N components, remaining are set to 1. If N == 3, `w` is set to 1.
     */
    /// @param args List of values for vector components.
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
    /// @brief Cross product (for 3D vectors).
    /// @return Cross product result as vec<T, N>.
    /// @param other Another vector.
    inline vec<T, N> operator^(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        else
            return x * other.y - y * other.x;
    }

    /// @brief Vector addition.
    /// @return Sum of vectors.
    /// @param other Another vector.
    inline vec<T, N> operator+(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x + other.x, y + other.y, z + other.z);
        else
            return vec<T, N>(x + other.x, y + other.y);
    }
    /// @brief Vector subtraction.
    /// @return Difference of vectors.
    /// @param other Another vector.
    inline vec<T, N> operator-(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x - other.x, y - other.y, z - other.z);
        else
            return vec<T, N>(x - other.x, y - other.y);
    }
    /// @brief Multiply by scalar.
    /// @return Scaled vector.
    /// @param f Scalar value.
    inline vec<T, N> operator*(T f) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x * f, y * f, z * f);
        else
            return vec<T, N>(x * f, y * f);
    }
    /// @brief Divide by scalar.
    /// @return Scaled vector.
    /// @param f Scalar value.
    inline vec<T, N> operator/(T f) const
    {
        if constexpr (N >= 3)
            return vec<T, N>(x / f, y / f, z / f);
        else
            return vec<T, N>(x / f, y / f);
    }
    /// @brief Dot product.
    /// @return Dot product result.
    /// @param other Another vector.
    inline T operator*(const vec<T, N> &other) const
    {
        if constexpr (N >= 3)
            return x * other.x + y * other.y + z * other.z;
        else
            return x * other.x + y * other.y;
    }
    /// @brief Access element by index (const).
    /// @return Element value.
    /// @param i Index.
    inline const T &operator[](int i) const
    {
        return raw[i];
    }
    /// @brief Access element by index (mutable).
    /// @return Reference to element.
    /// @param i Index.
    inline T &operator[](int i) { return raw[i]; }
    /// @brief Equality comparison.
    /// @return True if all elements are equal.
    /// @param other Another vector.
    bool operator==(const vec<T, N> &other) const noexcept
    {
        return std::equal(begin(), end(), other.begin(), other.end());
    }
    /// @brief Compute Euclidean normal (length) of vector.
    /// @return Vector magnitude.
    float norm() const
    {
        float sum = 0;
        for (int i = 0; i < N; ++i)
            sum += raw[i] * raw[i];
        return std::sqrt(sum);
    }
    /// @brief Normalize the vector to a given length.
    /// @throws std::runtime_error if norm() == 0.
    /// @param l Desired length (default 1).
    /// @return Reference to normalized vector.
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
// Type aliases for common vector sizes
typedef vec<float, 4> vec4f;
typedef vec<int, 4> vec4i;
typedef vec<double, 4> vec4d;

typedef vec<float, 3> vec3f;
typedef vec<double, 3> vec3d;
typedef vec<int, 3> vec3i;

typedef vec<float, 2> vec2f;
typedef vec<int, 2> vec2i;

/// @brief Generic matrix template for mathematical and graphics applications.
/**
    Supports addition, subtraction, scalar and matrix multiplication,
    determinant, transpose, and inversion operations.
*/
/// @tparam rows Number of rows.
/// @tparam cols Number of columns.
/// @tparam T Data type (default double).

template <size_t rows, size_t cols, typename T = double>
struct Matrix
{
    std::array<std::array<T, cols>, rows> data{}; ///< Internal matrix data.

    /// @brief Default constructor (zero-initialized).
    constexpr Matrix() = default;

    /// @brief Initialize matrix from a nested initializer list.
    /// @param values List of row data.
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
    /// @brief Row access (mutable).
    constexpr auto operator[](size_t row) -> std::array<T, cols> &
    {
        return data[row];
    }
    /// @brief Row access (const).
    constexpr auto operator[](size_t row) const -> const std::array<T, cols> &
    {
        return data[row];
    };
    /// @brief Add scalar to all elements (in-place).
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
    /// @brief Subtract scalar from all elements (in-place).
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
    /// @brief Multiply all elements by scalar (in-place).
    /*!
    * \verbatim
    A = | a11 a12 |    k = 3
        | a21 a22 |

    k * A = | 3*a11  3*a12 |
            | 3*a21  3*a22 |
    * \endverbatim
    */
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
    /// @brief Add another matrix (in-place).
    /*!
    * \verbatim
    A = | a11 a12 |    B = | b11 b12 |
        | a21 a22 |        | b21 b22 |

    A =     | a11+b11  a12+b12 |
            | a21+b21  a22+b22 |
    * \endverbatim
    */
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
    /// @brief Subtract another matrix (in-place).
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
    /// @brief Add scalar to all elements.
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
    /// @brief Subtract scalar from all elements.
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
    /// @brief Multiply all elements by scalar.
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
    /// @brief Divide all elements by scalar.
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
    /// @brief Matrix addition.
    /*!
    * \verbatim
    A = | a11 a12 |    B = | b11 b12 |
        | a21 a22 |        | b21 b22 |

    C =     | a11+b11  a12+b12 |
            | a21+b21  a22+b22 |
    * \endverbatim
    */
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
    /// @brief Matrix subtraction.
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

    /// @brief Matrix-matrix multiplication.
    /*!
    * \verbatim
    A = | a11 a12 |    B = | b11 b12 |
        | a21 a22 |        | b21 b22 |

    A * B = | a11*b11 + a12*b21   a11*b12 + a12*b22 |
            | a21*b11 + a22*b21   a21*b12 + a22*b22 |
    * \endverbatim
    */
    /// @tparam otherCols Number of columns in right-hand matrix.
    /// @return Resulting matrix.
    /// @param rhs Matrix to multiply by.

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

    /// @brief Matrix-vector multiplication.
    /// @return Resulting vector.
    /// @param vect Input vector.

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
    /// @brief Compute determinant of square matrix.
    ///@throws std::runtime_error if not square.
    /// @return Determinant value.

    double det()
    {
        static_assert(rows == cols, "Must be a square matrix!");
        Matrix<rows, cols, T> mat = *this;

        if (rows == 2)
            return (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]);

        double det = 1;
        T sign = 1;

        for (size_t i = 0; i < rows; ++i)
        {
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
            {

                double coef = mat[j][i] / mat[i][i];

                for (size_t k = i; k < rows; k++)
                {
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
    /**
     * @brief Transpose the matrix (swap rows and columns).
     * @details
     * \verbatim
     * A = | a11 a12 a13 |
     *     | a21 a22 a23 |
     *
     * A^T = | a11 a21 |
     *       | a12 a22 |
     *       | a13 a23 |
     * \endverbatim
     *
     * @return Transposed matrix.
     */
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
    /**
     * @brief Compute matrix inverse using Gaussian elimination.
     * @details
     * \verbatim
     * A = | a11 a12 |
     *     | a21 a22 |
     *
     * A^-1 = (1/det(A)) * |  a22  -a12 |
     *                     | -a21   a11 |
     * \endverbatim
     *
     * @return Inverse matrix, or std::nullopt if not invertible.
     */
    std::optional<Matrix<rows, cols, T>> inverse() const
    {
        static_assert(rows == cols, "Must be a square matrix!");
        Matrix<rows, cols, T> mat = *this;
        Matrix<rows, cols, T> inv{};
        for (size_t i = 0; i < rows; ++i)
            inv[i][i] = 1;

        for (size_t i = 0; i < rows; ++i)
        {
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
            {
                mat[i][k] = mat[i][k] / mat[i][i];
                inv[i][k] = inv[i][k] / mat[i][i];
            }
            for (size_t j = i + 1; j < rows; ++j)
            {

                double coef = mat[j][i] / mat[i][i];

                for (size_t k = i; k < rows; k++)
                {
                    mat[j][k] -= mat[i][k] * coef;
                    inv[j][k] -= inv[i][k] * coef;
                }
            }
        }

        for (int i = rows - 1; i >= 0; --i)
        {
            for (int j = i - 1; j >= 0; --j)
            {
                double coef = mat[j][i] / mat[i][i];

                for (size_t k = 0; k < rows; k++)
                {
                    mat[j][k] -= mat[i][k] * coef;
                    inv[j][k] -= inv[i][k] * coef;
                }
            }
        }
        return inv;
    }
};
// Type aliases for common matrix sizes
typedef Matrix<3, 3> mat3;
typedef Matrix<4, 4, float> mat4;

#endif // MATH_CORE_H