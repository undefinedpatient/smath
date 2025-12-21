#ifndef SMATH_MAT_HPP
#define SMATH_MAT_HPP

#include "smath/vec.hpp"
#include <cmath>
#include <concepts>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace smath {
/*
    M := row, N := column
    Mat is a column major matrix
    Meaning the layout is:
    [r0,r0,r0,r1,r1,r1,r2,r2,r2]
*/
template <unsigned int M, unsigned int N, class T>
    requires(std::is_arithmetic<T>::value && M < 16 && N < 16)
class Mat {
  private:
    T data[M * N]{};

  public:
    /***************************************
        Constructors
    ****************************************/
    Mat() : data{} {}
    Mat(T value) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                data[n * M + m] = value;
            }
        }
    }
    static Mat<M, M, T> identity() {
        Mat<M, M, T> i = {};
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                if (n == m)
                    i.data[n * M + m] = static_cast<T>(1.0f);
            }
        }
        return i;
    }
    static Mat<M, N, T> full(const T &value) {
        Mat<M, N, T> result = {};
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result.data[n * M + m] = value;
            }
        }
        return result;
    }
    template <class... Us>
        requires((sizeof...(Us) == M * N) &&
                 (std::convertible_to<Us, T> && ...))
    Mat(Us... args) : data{static_cast<T>(args)...} {}
    // Copy constructor
    Mat(const Mat &other) = default;
    Mat(const Vec<M, T> &vec) {
        std::memcpy(this->data, &(vec.data[0]), M * sizeof(T));
    }
    // Move constructor
    Mat(Mat &&other) noexcept = default;
    Mat(const Vec<M, T> &&vec) noexcept {
        if (N != 1)
            throw std::invalid_argument("Invalid Matrix initialisation.");
        std::memcpy(this->data, &(vec.data[0]), M * sizeof(T));
    }
    // Copy assignment
    Mat<M, N, T> &operator=(const Mat<M, N, T> &other) = default;
    // Move assignment
    Mat<M, N, T> &operator=(Mat<M, N, T> &&other) noexcept = default;

    /***************************************
        Getters
    ****************************************/

    /**
     * @brief Get the column of the matrix by index.
     * @param index of type unsigned int.
     */
    T &operator[](unsigned int index) {
        if (index >= M * N) {
            throw std::out_of_range("Index out of bound");
        }
        return data[index];
    }
    const T &operator[](unsigned int index) const {
        if (index >= M * N) {
            throw std::out_of_range("Index out of bound");
        }
        return data[index];
    }
    /**
     * @return String representation of matrix.
     */
    std::string toString() const {
        std::string str = "";
        for (unsigned int n = 0; n < N; n++) {
            str += "[";
            for (unsigned int m = 0; m < M; m++) {
                str += std::to_string(this->data[n * M + m]);
                if (m != M - 1)
                    str += ",";
            }
            str += "]\n";
        }
        return str;
    }
    /***************************************
        Operations
    ****************************************/

    /**
     * @return An array of length N, consists of Vec<M,T>.
     */
    std::array<Vec<M, T>, N> toVectors() const {
        std::array<Vec<M, T>, N> vec;
        for (unsigned int n = 0; n < N; n++) {
            T temp_data[M];
            for (unsigned int m = 0; m < M; m++) {
                temp_data[m] = this->data[M * n + m];
            }
            vec[n] = Vec<M, T>(temp_data);
        }
        return vec;
    }

    template <unsigned int K>
    Mat<M, K, T> cross(const Mat<N, K, T> &other) const {
        Mat<M, K, T> result{};
        for (unsigned int k = 0; k < K; k++) {
            for (unsigned int m = 0; m < M; m++) {
                T temp = 0;
                for (unsigned int n = 0; n < N; n++) {
                    temp += (*this)[n * M + m] * other[k * N + n];
                }
                result[k * M + m] = temp;
            }
        }

        return result;
    }

    /**
     * @return A new transposed version of matrix.
     */
    Mat<N, M, T> transpose() const {
        Mat<N, M, T> copy{};
        for (unsigned int m = 0; m < M; m++) {
            for (unsigned int n = 0; n < N; n++) {
                copy[m * N + n] = (*this)[n * M + m];
            }
        }
        return copy;
    }
    /**
     * @return A new adjoint version of matrix.
     */
    Mat<M, N, T> adjoint() const {
        Mat<M, N, T> cofactor{};

        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                const int sign = ((n + m) % 2) ? -1 : 1;
                cofactor[M * n + m] =
                    sign * this->subMatrixAt(n, m).determinant();
            }
        }
        return cofactor.transpose();
    }
    /**
     * @return A new inverse version of matrix.
     */
    Mat<M, N, T> inverse() const { return (1 / determinant()) * adjoint(); }
    /**
     * @return Sub-matrix without the column c and row r.
     */
    Mat<M - 1, N - 1, T> subMatrixAt(unsigned int c, unsigned int r) const {
        if constexpr (M <= 1 || N <= 1) {
            throw std::invalid_argument(
                "Invalid Argument: Matrix is too small");
        }
        if (c >= N || r >= M) {
            throw std::out_of_range("Invalid Range");
        }
        Mat<M - 1, N - 1, T> sub_matrix{};
        for (unsigned int n = 0; n < N - 1; n++) {
            for (unsigned int m = 0; m < M - 1; m++) {
                if (m >= r && n >= c) {
                    sub_matrix[n * (M - 1) + m] =
                        (*this)[(n + 1) * M + (m + 1)];
                } else if (m >= r) {
                    sub_matrix[n * (M - 1) + m] = (*this)[(n)*M + (m + 1)];
                } else if (n >= c) {
                    sub_matrix[n * (M - 1) + m] = (*this)[(n + 1) * M + m];
                } else {
                    sub_matrix[n * (M - 1) + m] = (*this)[(n)*M + (m)];
                }
            }
        }
        return sub_matrix;
    }

    // Notes: Since it is a function generated by the compiler, "else" must
    // be used to indicate non-independent event, and "constexpr" must be
    // used for compilation time evaluation.
    /**
     * @brief Get the determinant of the corresponding matrix.
     * @return T the type of data store in the matrix.
     */
    T determinant() const {
        if constexpr (M != N) {
            return std::range_error(
                "Range Error: Matrix is not a square matrix!");
        }
        // If the matrix is 1x1, return the value
        else if constexpr (M == 1) {
            return data[0];
        } else if constexpr (M == 2) {
            // Nothing else just to save template slot
            return data[0] * data[3] - data[1] * data[2];
        } else {
            T sum = 0;
            for (unsigned int n = 0; n < N; n++) {
                // Refer to determinant method, + - + - + for a 5x5 matrix
                Mat<M - 1, N - 1, T> sub = this->subMatrixAt(n, 0);
                T det = sub.determinant();
                if (n % 2 == 0) {
                    sum += (*this)[M * n] * det;
                } else {
                    sum -= (*this)[M * n] * det;
                }
            }
            return sum;
        }
    }

    /**
     * @brief 2D translation for 3x3 homogeneous matrix
     */
    // template <typename U = T>
    //     requires(M == 3 && N == 3)
    Mat<3, 3, T> translate(const Vec<2, T> vec2) const requires (M==3&&N==3){
        const Mat<3, 3, T> translation = {1, 0,       0,       0, 1,
                                          0, vec2[0], vec2[1], 1};
        return (*this) * translation;
    }

    /**
     * @brief 3D translation for 4x4 homogeneous matrix
     */
    // template <typename U = T>
    //     requires(M == 4 && N == 4)
    Mat<4, 4, T> translate(const Vec<3, T> vec3) const requires (M==4&&N==3){
        const Mat<4, 4, T> translation = {
            1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, vec3[0], vec3[1], vec3[2], 1};
        return (*this) * translation;
    }

    /**
     * @brief 2D rotation on 2x2 matrix
     */
    // template <typename U = T>
    //     requires(M == 2 && N == 2)
    Mat<2, 2, T> rotate(const T &radian) const requires (M==2&&N==2){
        return Mat<2, 2, T>(std::cos(radian), std::sin(radian),
                            -std::sin(radian), std::cos(radian)) *
               (*this);
    }
    /**
     * @brief 2D rotation on 3x3 homogeneous matrix
     */
    template <typename U = T>
        requires(M == 3 && N == 3)
    Mat<3, 3, T> rotate(const T &radian) const requires (M==3&&N==3){
        return Mat<3, 3, T>(std::cos(radian), std::sin(radian),
                            -std::sin(radian), std::cos(radian)) *
               (*this);
    }

    /**
     * @brief 3D rotation on 3x3 matrix
     */
    // template <typename U = T>
    //     requires(M == 3 && N == 3)
    Mat<3, 3, T> rotate(const T &radian,
                        const Vec<3, T> &axis = {1, 0, 0}) const requires (M==3&&N==3){
        const auto unit = axis.normalise();
        using namespace std;
        Mat<3, 3, T> rotation = {
            (unit[0] * unit[0])(1 - cos(radian)) + cos(radian),
            (unit[0] * unit[1])(1 - cos(radian)) + unit[2] * sin(radian),
            (unit[0] * unit[2])(1 - cos(radian)) - unit[1] * sin(radian),

            (unit[0] * unit[1])(1 - cos(radian)) - unit[2] * sin(radian),
            (unit[1] * unit[1])(1 - cos(radian)) + cos(radian),
            (unit[1] * unit[2])(1 - cos(radian)) + unit[0] * sin(radian),

            (unit[0] * unit[2])(1 - cos(radian)) + unit[1] * sin(radian),
            (unit[1] * unit[2])(1 - cos(radian)) - unit[0] * sin(radian),
            (unit[2] * unit[2])(1 - cos(radian)) + cos(radian),
        };
        return rotation * (*this);
    }

    /**
     * @brief 3D rotation on 4x4 homogeneous matrix
     */
    // template <typename U = T>
    //     requires(M == 4 && N == 4)
    Mat<4, 4, T> rotate(const T &radian, const Vec<4, T> &axis = {1, 0, 0},
                        Vec<3, T> pivot = {0, 0, 0}) const requires (M==4&&N==4){
        const auto unit = axis.normalise();
        using namespace std;
        Mat<4, 4, T> rotation = {
            (unit[0] * unit[0])(1 - cos(radian)) + cos(radian),
            (unit[0] * unit[1])(1 - cos(radian)) + unit[2] * sin(radian),
            (unit[0] * unit[2])(1 - cos(radian)) - unit[1] * sin(radian),
            0,

            (unit[0] * unit[1])(1 - cos(radian)) - unit[2] * sin(radian),
            (unit[1] * unit[1])(1 - cos(radian)) + cos(radian),
            (unit[1] * unit[2])(1 - cos(radian)) + unit[0] * sin(radian),
            0,

            (unit[0] * unit[2])(1 - cos(radian)) + unit[1] * sin(radian),
            (unit[1] * unit[2])(1 - cos(radian)) - unit[0] * sin(radian),
            (unit[2] * unit[2])(1 - cos(radian)) + cos(radian),
            0,

            0,
            0,
            0,
            1};
        return rotation * (*this).translate(-pivot);
    }

    friend Mat<M, N, T> operator+(const Mat<M, N, T> &a,
                                  const Mat<M, N, T> &b) {
        Mat<M, N, T> result = Mat<M, N, T>();
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result.data[m + n * M] = a.data[m + n * M] + b.data[m + n * M];
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator-(const Mat<M, N, T> &a,
                                  const Mat<M, N, T> &b) {
        Mat<M, N, T> result = Mat<M, N, T>();
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result.data[m + n * M] = a.data[m + n * M] - b.data[m + n * M];
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator*(const Mat<M, N, T> &a,
                                  const Mat<M, N, T> &b) {
        Mat<M, N, T> result = Mat<M, N, T>(a);
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] *= b[n * M + m];
            }
        }
        return result;
    }

    friend Mat<M, N, T> operator*(const T &a, const Mat<M, N, T> &b) {
        Mat<M, N, T> result = b;
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] *= a;
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator*(const Mat<M, N, T> a, const T &b) {
        Mat<M, N, T> result = a;
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] *= b;
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator/(const Mat<M, N, T> &a,
                                  const Mat<M, N, T> &b) {
        Mat<M, N, T> result = Mat<M, N, T>(a);
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] /= b[n * M + m];
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator/(const Mat<M, N, T> a, const T &b) {
        return a * (1 / b);
    }
    friend Mat<M, N, T> operator%(const Mat<M, N, T> &a,
                                  const Mat<M, N, int> &b) {
        Mat<M, N, T> result = Mat<M, N, T>(a);
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] %= b[n * M + m];
            }
        }
        return result;
    }
    friend Mat<M, N, T> operator%(const Mat<M, N, T> a, const int &b) {
        Mat<M, N, T> result = a;
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                result[n * M + m] %= b;
            }
        }
        return result;
    }
    Mat<M, N, T> &operator+=(const Mat<M, N, T> &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] += other[n * M + m];
            }
        }
    }
    Mat<M, N, T> &operator-=(const Mat<M, N, T> &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] -= other[n * M + m];
            }
        }
    }
    Mat<M, N, T> &operator*=(const Mat<M, N, T> &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] *= other[n * M + m];
            }
        }
    }
    Mat<M, N, T> &operator*=(const T &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] *= other;
            }
        }
    }
    Mat<M, N, T> &operator/=(const Mat<M, N, T> &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] /= other[n * M + m];
            }
        }
    }
    Mat<M, N, T> &operator/=(const T &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] /= other;
            }
        }
    }
    Mat<M, N, T> &operator%=(const Mat<M, N, T> &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] %= other[n * M + m];
            }
        }
    }
    Mat<M, N, T> &operator%=(const T &other) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                (*this)[n * M + m] %= other;
            }
        }
    }
    friend bool operator==(const Mat<M, N, T> &a, const Mat<M, N, T> &b) {
        for (unsigned int n = 0; n < N; n++) {
            for (unsigned int m = 0; m < M; m++) {
                if (a.data[n * M + m] != b.data[n * M + m]) {
                    return false;
                }
            }
        }
        return true;
    }
    friend bool operator!=(const Mat<M, N, T> &a, const Mat<M, N, T> &b) {
        return !(a == b);
    }
    friend std::ostream &operator<<(std::ostream &o, const Mat<M, N, T> mat) {
        o << mat.toString();
        return o;
    }
};

using Mat4f = Mat<4, 4, float>;
using Mat3f = Mat<3, 3, float>;
using Mat2f = Mat<2, 2, float>;
using Mat1f = Mat<1, 1, float>;
using Mat4d = Mat<4, 4, double>;
using Mat3d = Mat<3, 3, double>;
using Mat2d = Mat<2, 2, double>;
using Mat1d = Mat<1, 1, double>;

/**
 * @return Calculate the outer product of vectors.
 */
template <unsigned int N, class T>
Mat<N, N, T> outer_product(const Vec<N, T> left, const Vec<N, T> right) {
    Mat<N, 1, T> l{left};
    Mat<1, N, T> r{right};
    return l.cross(r.transpose());
}

template <class T>
Mat<4, 4, T> look_at(const Vec<3, T> &eye, const Vec<3, T> &target,
                     const Vec<3, T> &up) {
    const Vec<3, T> forward = static_cast<Vec<3, T>>(target - eye).normalise();
    const Vec<3, T> right = forward.cross(up.normalise());
    const Vec<3, T> true_up = right.cross(forward);

    return Mat<4, 4, T>{
        right[0],   right[1],   right[2],   0, up[0], up[1], up[2], 0,
        forward[0], forward[1], forward[2], 0, 0,     0,     0,     1};
}

/**
 * @brief This is a function generating the perspective projection matrix based
 * on right-hand coordinate system, assuming camera pointing along the -z axis.
 * Note that near and far are absolute value indicating the distance from the camera.
 */
template <class T>
    requires (std::is_arithmetic<T>::value)
Mat<4, 4, T> perspective(const T &aspect_ratio, const T &fov, const T &near,
                         const T &far) {
    const T n = -near;
    const T f = -far;
    return Mat<4, 4, T>{1 / (aspect_ratio * std::tan(fov / 2)),
                        0,
                        0,
                        0,
                        0,
                        1 / std::tan(fov / 2),
                        0,
                        0,
                        0,
                        0,
                        (f + n) / (n - f),
                        1,
                        0,
                        0,
                        -(2 * f * n) / (n - f)};
}
/**
 * @brief This is a alternative function generating the perspective projection matrix based
 * on right-hand coordinate system, assuming camera pointing along the -z axis.
 * Note that near and far are absolute value indicating the distance from the camera.
 */
template <class T>
Mat<4, 4, T> perspective(const T &left, const T &right, const T &top,
                         const T &bottom, const T &near, const T &far) {
    const T n = -near;
    const T f = -far;
    return Mat<4,4,T>{
        (2*n)/(right-left),0,0,0,
        0,(2*n)/(top-bottom),0,0,
        -((right+left)/(right-left)),-((top+bottom)/(top-bottom)),2*((f+n)/(n-f)),1,
        0,0,-((2*f*n)/(n-f)),0
    };
}
/**
 * @brief This is a function generating the orthographic projection matrix based
 * on right-hand coordinate system, assuming camera pointing along the -z axis.
 * Note that near and far are absolute value indicating the distance from the camera.
 */
template <class T>
Mat<4, 4, T> orthgraphic(const T &left, const T &right, const T &top,
                         const T &bottom, const T &near, const T &far) {
    const T n = -near;
    const T f = -far;
    return Mat<4, 4, T>{2 / (right - left),
                        0,
                        0,
                        0,
                        0,
                        2 / (top - bottom),
                        0,
                        0,
                        0,
                        0,
                        2 / (n- f),
                        0,
                        -(left + right) / (right - left),
                        -(top + bottom) / (top - bottom),
                        -(n+ f) / (n- f),
                        1};
};

} // namespace smath
#endif
