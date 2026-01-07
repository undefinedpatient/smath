#ifndef SMATH_QUAT_HPP
#define SMATH_QUAT_HPP

#include "mat.hpp"
#include <cmath>
#include <concepts>
#include <initializer_list>
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
    Quat() : data{} {}
    Quat(const T &q0, const T &q1, const T &q2, const T &q3)
        : data{q0, q1, q2, q3} {}
    Quat(const T &real, const Vec<3, T> &imaginary)
        : data{real, imaginary[0], imaginary[1], imaginary[2]} {}
    // Copy constructor
    Quat(const Quat &other) = default;
    // Move constructor
    Quat(Quat &&other) = default;
    // Copy assignment
    Quat &operator= (const Quat &other) = default;
    // Move assignment
    Quat &operator= (Quat && other) = default;
    // Initializer list
    Quat(std::initializer_list<T> values) {
        std::memcpy(data, values.begin(), sizeof(T)*4);
    }

    static Quat<T> identity() {
        return Quat<T>{1.0f,0.0f,0.0f,0.0f};

    }

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
     * @return scalar components
     */
    const T scalar() const {
        return data[0];
    }
    /**
     * @return imaginary components
     */
    const Vec<3,T> vector() const {
        return Vec<3,T>{data[1], data[2], data[3]};
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
    Quat<T> mul(const Quat<T> &other) const{
        return Quat<T>{
            data[0]*other[0]-data[1]*other[1]-data[2]*other[2]-data[3]*other[3],
            data[0]*other[1]+data[1]*other[0]+data[2]*other[3]-data[3]*other[2],
            data[0]*other[2]+data[2]*other[0]+data[3]*other[1]-data[1]*other[3],
            data[0]*other[3]+data[3]*other[0]+data[1]*other[2]-data[2]*other[1]
        };
    }
    T dot(const Quat<T> &other) const{
        return data[0]*other[0]+data[1]*other[1]+data[2]*other[2]+data[3]*other[3];
    }
    Quat<T> conjugate() const {
        return Quat{data[0], -data[1], -data[2], -data[3]};
    }
    /**
     * @return Normalized Quaternion of the operand. Raise error when
     * encounter a zero-quaternion
     */
    Quat<T> normalize() const {
        T divisor = length();
        if(divisor==0){
            throw std::logic_error("Cannot normalize a zero-quaternion.");
        }
        return (*this)/divisor;
    }
    /**
     * @return Normalized Quaternion of the operand. Return zero-quaternion when
     * encounter a zero-quaternion
     */
    Quat<T> normalize_or_zero() const {
        T divisor = length();
        if(divisor==0){
            return {0,0,0,0};
        }
        return (*this)/divisor;
    }
    /**
     * @return Normalized Quaternion of the operand. Return default-quaternion when
     * encounter a zero-quaternion
     */
    Quat<T> normalize_or_one() const {
        T divisor = length();
        if(divisor==0){
            return {1,0,0,0};
        }
        return (*this)/divisor;
    }
    /**
     * @return length of the Quaternion.
     */
    T length() const {
        return std::sqrt(length2());
    }
    /**
     * @return squared length of the Quaternion.
     */
    T length2() const {
        return data[0] * data[0] + data[1] * data[1] + data[2] * data[2] +
               data[3] * data[3];
    }
    Quat<T> inverse() const { return this->conjugate() / this->length2(); }
    /**
     * @brief Convert Quaternion into Mat3x3
     */
    Mat<3, 3, T> to_mat3() const {
        auto q = this->normalize_or_one();
        return Mat<3, 3, T>{static_cast<T>(2.0f) * (q[0] * q[0] + q[1] * q[1] - static_cast<T>(0.5)),
                            static_cast<T>(2.0f) * (q[0] * q[3] + q[1] * q[2]),
                            static_cast<T>(2.0f) * (q[1] * q[3] - q[0] * q[2]),
                            static_cast<T>(2.0f) * (q[1] * q[2] - q[0] * q[3]),
                            static_cast<T>(2.0f) * (q[0] * q[0] + q[2] * q[2] - static_cast<T>(0.5)),
                            static_cast<T>(2.0f) * (q[0] * q[1] + q[2] * q[3]),
                            static_cast<T>(2.0f) * (q[0] * q[2] + q[1] * q[3]),
                            static_cast<T>(2.0f) * (q[2] * q[3] + q[0] * q[1]),
                            static_cast<T>(2.0f) * (q[0] * q[0] + q[3] * q[3] - static_cast<T>(0.5))};
    }
    /**
     * @brief Convert Quaternion into Mat4x4
     */
    Mat<4, 4, T> to_mat4() const {
        return to_mat3().to_homogeneous();
    }
    /**
     * @brief Convert an Eular rotation matrix to Quaternion. Using method
     * described in "Accurate Computation of Quaternions from Rotation Matrices" in January 2019.
     */
    Quat<T> from_mat3(const Mat<3, 3, T>& matrix, const T& threshold = 0) const {
        const Mat<3, 3, T> &m = matrix;
        const T q_0 = (m[0] + m[4] + m[8] > threshold)
                          ? (1 / 2) * sqrt(1 + m[0] + m[4] + m[8])
                          : (1 / 2) * sqrt(((m[5] - m[7]) * (m[5] - m[7]) +
                                            (m[6] - m[2]) * (m[6] - m[2]) +
                                            (m[1] - m[3]) * (m[1] - m[3])) /
                                           (3 - m[0] - m[4] - m[8]));
        const T q_1 = (m[0] - m[4] - m[8] > threshold)
                          ? (1 / 2) * sqrt(1 + m[0] - m[4] - m[8])
                          : (1 / 2) * sqrt(((m[5] - m[7]) * (m[5] - m[7]) +
                                            (m[3] + m[1]) * (m[3] + m[1]) +
                                            (m[2] + m[6]) * (m[2] + m[6])) /
                                           (3 - m[0] + m[4] + m[8]));
        const T q_2 = (-m[0] + m[4] - m[8] > threshold)
                          ? (1 / 2) * sqrt(1 - m[0] + m[4] - m[8])
                          : (1 / 2) * sqrt(((m[6] - m[2]) * (m[6] - m[2]) +
                                            (m[3] + m[1]) * (m[3] + m[1]) +
                                            (m[7] + m[5]) * (m[7] + m[5])) /
                                           (3 + m[0] - m[4] + m[8]));
        const T q_3 = (-m[0] - m[4] + m[8] > threshold)
                          ? (1 / 2) * sqrt(1 - m[0] - m[4] + m[8])
                          : (1 / 2) * sqrt(((m[1] - m[3]) * (m[1] - m[3]) +
                                            (m[6] + m[2]) * (m[6] + m[2]) +
                                            (m[5] + m[7]) * (m[5] + m[7])) /
                                           (3 + m[0] + m[4] - m[8]));
        ;
        return Quat<T>{q_0, q_1, q_2, q_3};
    }
    Quat<T> from_mat4(const Mat<4, 4, T>& mat) const {
        return from_mat3(mat.to_mat3());
    }
    bool all() const{
        for (unsigned int i = 0; i<4; i++){
            if(!data[i])
                return false;
        }
        return true;
    }
    bool any() const{
        for (unsigned int i = 0; i<4; i++){
            if(data[i])
                return true;
        }
        return false;
    }
    bool none() const{
        return !any();
    }
    /***************************************
            Operators Overload
    ****************************************/

    /**
     * @brief Custom boolean casting for quaternion. This require the element in the Quat to be able to convert to bool.
     * @return True if and only if all vector elements are True.
     */
    explicit operator bool() const {
        for (unsigned int i = 0; i < 4; i++) {
            if (!(*this)[i])
                return false;
        }
        return true;
    }
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

    /***************************************
            Relational Operators
    ****************************************/
    friend Quat<unsigned int> operator==(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] == b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator!=(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] != b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator<(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] < b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator>(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] > b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator<=(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] <= b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator>=(const Quat<T> &a, const Quat<T> &b) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i] >= b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Quat<unsigned int> operator!(const Quat<T> &a) {
        Quat<unsigned int> result{};
        for (unsigned int i = 0; i <4; i++) {
            result[i] = (a[i]) ? 1 : 0;
        }
        return result;
    }
    friend std::ostream &operator<<(std::ostream &o, const Quat<T> &quat) {
        o << quat.to_string();
        return o;
    }
};
template<class T>
Quat<T> slerp(const Quat<T> &a, const Quat<T> &b, const T& t){
    using namespace std;
    const T angle = acos(a.dot(b));
    return a*(sin((1-t)*angle)/sin(angle))+b*(sin(t*angle)/sin(angle));
}
} // namespace smath
#endif
