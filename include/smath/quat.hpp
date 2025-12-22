#ifndef SMATH_QUAT_HPP
#define SMATH_QUAT_HPP

#include "smath/mat.hpp"
#include <cmath>
#include <concepts>
#include <string>
namespace smath {
template <class T>
    requires(std::is_arithmetic_v<T>)
class Quat {
  private:
    T data[4];

  public:
    /***************************************
            Constructors
    ****************************************/
    Quat() : data{} {};
    Quat(T q0, T q1, T q2, T q3) : data{q0, q1, q2, q3} {};
    // Copy constructor
    Quat(const Quat &other) = default;
    // Move constructor
    Quat(Quat &&other) = default;
    // Copy assignment
    Quat &operator= (const Quat &other) = default;
    // Move assignment
    Quat &operator= (Quat && other) = default;

    /***************************************
            Getters
    ****************************************/
    /**
     * @brief Operator Overloading for accessing data array size of N
     * @param i zero-based index
     * @return data[i]
     */
    T &operator[](int i) {
        if (i < 0 || i > 3)
            throw std::out_of_range("Index out of bound");
        return data[i];
    }
    /**
     * @brief Operator Overloading for accessing data array size of N
     * @param i zero-based index
     * @return data[i]
     */
    const T &operator[](int i) const {
        if (i < 0 || i > 3)
            throw std::out_of_range("Index out of bound");
        return data[i];
    }
    /**
     * @return String representation of Quaternion
     */
    constexpr std::string to_string() const {
        std::string str = "Quat (";
        for (unsigned int i = 0; i < 4; i++) {
            str += std::to_string(data[i]);
            switch (i){
                case 0:
                    str += "+ ";
                    break;
                case 1:
                    str += "i+ ";
                    break;
                case 2:
                    str += "j+ ";
                    break;
                case 3:
                    str += "k";
                    break;
            }
        }
        str += ")";
        return str;
    }

    /***************************************
           Operations
    ****************************************/
    Quat<T> conjugate() const {
        return Quat{data[0], -data[1], -data[2], -data[3]};
    }
    T norm() const {
        return std::sqrt(data[0] * data[0] + data[1] * data[1] +
                         data[2] * data[2] + data[3] * data[3]);
    }
    T norm2() const {
        return data[0] * data[0] + data[1] * data[1] + data[2] * data[2] +
               data[3] * data[3];
    }
    Quat<T> inverse() const { return this->conjugate() / this->norm2(); }
    Mat<3, 3, T> to_mat3() const {
        return Mat<3, 3, T>{2 * (data[0] * data[1] - 0.5),
                            2 * (data[0] * data[3] + data[1] * data[2]),
                            2 * (data[1] * data[3] - data[0] * data[2]),
                            2 * (data[1] * data[2] - data[0] * data[3]),
                            2 * (data[0] * data[0] + data[2] * data[2] - 0.5),
                            2 * (data[0] * data[1] + data[2] * data[3]),
                            2 * (data[0] * data[2] + data[1] * data[3]),
                            2 * (data[2] * data[3] + data[0] * data[1]),
                            2 * (data[0] * data[0] + data[3] * data[3] - 0.5)};
    }
    Mat<4, 4, T> to_mat4() const {
        return Mat<4, 4, T>{2 * (data[0] * data[1] - 0.5),
                            2 * (data[0] * data[3] + data[1] * data[2]),
                            2 * (data[1] * data[3] - data[0] * data[2]),
                            0,
                            2 * (data[1] * data[2] - data[0] * data[3]),
                            2 * (data[0] * data[0] + data[2] * data[2] - 0.5),
                            2 * (data[0] * data[1] + data[2] * data[3]),
                            0,
                            2 * (data[0] * data[2] + data[1] * data[3]),
                            2 * (data[2] * data[3] + data[0] * data[1]),
                            2 * (data[0] * data[0] + data[3] * data[3] - 0.5),
                            0,
                            0,
                            0,
                            0,
                            1};
    }
    /**
     * @brief Convert an Eular rotation matrix to Quaternion.
     */
    Quat<T> from_mat3(Mat<3, 3, T> mat) const {
        T trace = mat.trace();
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            if(i==0){
                result[i] = std::sqrt((trace+1)/4);
                continue;
            }
            result[i] = std::sqrt(mat[4 * i] / 2 + (1 - trace) / 4);
        }
        return result;
    }
    Quat<T> from_mat4(Mat<4, 4, T> mat) const {
        T trace = mat.trace();
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            if(i==0){
                result[i] = std::sqrt((trace+1)/4);
                continue;
            }
            result[i] = std::sqrt(mat[5 * i] / 2 + (1 - trace) / 4);
        }
        return result;
    }
    /***************************************
            Operators Overload
    ****************************************/

    /**
     * @brief Unary - operator, simply flip all element.
     */
    friend Quat<T> operator-(const Quat<T> &a) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = -a[i];
        }
        return result;
    }

    friend Quat<T> operator+(const Quat<T> &a, const Quat<T> &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a[i] + b[i];
        }
        return result;
    };
    friend Quat<T> operator-(const Quat<T> &a, const Quat<T> &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a[i] - b[i];
        }
        return result;
    }
    friend Quat<T> operator*(const Quat<T> &a, const Quat<T> &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a[i] * b[i];
        }
        return result;
    }
    friend Quat<T> operator*(const Quat<T> &a, const T &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a[i] * b;
        }
        return result;
    }
    friend Quat<T> operator*(const T &a, const Quat<T> &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a * b[i];
        }
        return result;
    }
    friend Quat<T> operator/(const Quat<T> &a, const T &b) {
        return a * (1 / b);
    }
    friend Quat<T> operator%(const Quat<T> &a, const T &b) {
        Quat<T> result{};
        for (unsigned int i = 0; i < 4; i++) {
            result[i] = a[i] % b;
        }
        return result;
    }
    Quat<T> &operator+=(const Quat<T> &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] += other[i];
        }
        return *this;
    };
    Quat<T> &operator-=(const Quat<T> &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] -= other[i];
        }
        return *this;
    };
    Quat<T> &operator*=(const Quat<T> &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] *= other[i];
        }
        return *this;
    };
    Quat<T> &operator*=(const T &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] *= other;
        }
        return *this;
    };
    Quat<T> &operator/=(const Quat<T> &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] /= other[i];
        }
        return *this;
    };
    Quat<T> &operator/=(const T &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] /= other;
        }
        return *this;
    };
    Quat<T> &operator%=(const Quat<T> &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] %= other[i];
        }
        return *this;
    };
    Quat<T> &operator%=(const T &other) {
        for (unsigned int i = 0; i < 4; i++) {
            (*this)[i] %= other;
        }
        return *this;
    };

};
} // namespace smath
#endif