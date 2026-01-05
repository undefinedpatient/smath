#ifndef SMATH_VEC_HPP
#define SMATH_VEC_HPP

#include <cmath>
#include <concepts>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <cstring>

namespace smath {
template <unsigned int N, class T>
    requires(std::is_arithmetic_v<T> && N <= 32)
class Vec {
  private:
    T data[N];

  public:
    /***************************************
            Constructors
    ****************************************/
    Vec() : data{} {};


    template <class... Us>
        requires(sizeof...(Us) == N && (std::convertible_to<Us, T> && ...))
    Vec(Us... args) : data{static_cast<T>(args)...} {}
    Vec(T value) {
        for(unsigned int i = 0; i< N; i++){
            data[i] = value;
        }
    }
    explicit Vec(const T *arr) {
        std::memcpy(this->data, arr, N*(sizeof(T)));
    }
    // Copy constructor
    Vec(const Vec &other) = default;
    // Move consturctor
    Vec(Vec &&other) noexcept = default;
    // Copy assignment
    Vec &operator=(const Vec<N, T> &other) = default;
    // Move assignment
    Vec &operator=(Vec<N, T> &&other) noexcept = default;

    // Initializer list
    Vec(std::initializer_list<T> values) {
        if(values.size()!=N){
            throw std::invalid_argument("Number of values mismatched target vector size.");
        }
        std::memcpy(data,values.begin(),sizeof(T)*N);
    }
    /***************************************
            Getters
    ****************************************/

    /**
     * @brief Operator Overloading for accessing data array size of N.
     * @param i zero-based index
     * @return data[i]
     */
    T &operator[](int i) {
        if (i < 0 || i > N - 1)
            throw std::out_of_range("Index out of bound");
        return data[i];
    }
    /**
     * @brief Operator Overloading for accessing data array size of N.
     * @param i zero-based index
     * @return data[i]
     */
    const T &operator[](int i) const {
        if (i < 0 || i > N - 1)
            throw std::out_of_range("Index out of bound");
        return data[i];
    }
    /**
     * @return String representation of vector.
     */
    constexpr std::string to_string() const {
        std::string str = "Vec" + std::to_string(N) + "<";
        for (unsigned int i = 0; i < N; i++) {
            str += std::to_string(data[i]);
            if (i != N - 1)
                str += ", ";
        }
        str += ">";
        return str;
    }

    /**
     * @brief Swizzling-like accessor.
     */
    auto operator[](const unsigned int index, auto... indices) const requires(sizeof...(indices)>0){
        return Vec<sizeof...(indices)+1, T>{(*this)[index], (*this)[indices]...};
    }
    /***************************************
            Operations
    ****************************************/

    /**
     * @return Length of the vector.
     */
    T length() const {
        T len = 0;
        for (unsigned int i = 0; i < N; i++) {
            len += data[i] * data[i];
        }
        return static_cast<T>(std::sqrt(len));
    }
    /**
     * @return Squared length of the vector.
     */
    T length2() const {
        T len = 0;
        for (int i = 0; i < N; i++) {
            len += data[i] * data[i];
        }
        return len;
    }
    /**
     * @return Classic Cross Product.
     */
    Vec<3, T> cross(const Vec<3, T> &other) const requires (N==3) {
        return Vec<3, T>{(*this)[1] * other[2] - (*this)[2] * other[1],
                         -(*this)[0] * other[2] + (*this)[2] * other[0],
                         (*this)[0] * other[1] - (*this)[1] * other[0]};
    }
    /**
     * @return Classic Dot Product.
     */
    T dot(const Vec<N, T> &other) const {
        T total = 0;
        for (unsigned int i = 0; i < N; i++) {
            total += (*this)[i] * other[i];
        }
        return total;
    }
    Vec<N+1,T> expand(const T& value) const {
        Vec<N+1,T> result{};
        std::memcpy(result->data, this->data, N*sizeof(T));
        result[N]=value;
        return result;
    }
    template <unsigned int M>
    Vec<N+M,T> combine(const Vec<M,T> other) const {
        Vec<N+M,T> result{};
        std::memcpy(result->data, this->data, N*sizeof(T));
        std::memcpy(result->data+N*sizeof(T), other.data, M*sizeof(T));
        return result;
    }
    /**
     * @return Normalized Vector of the operand. Raise error when encounter a zero-vector
     */
    Vec<N, T> normalize() const {
        Vec<N, T> temp{};
        const T length = this->length();
        if (length == 0)
            throw std::logic_error("Cannot normalize a zero-vector.");
        for (unsigned int i = 0; i < N; i++) {
            temp[i] = (*this)[i] / length;
        }
        return temp;
    }
    /**
     * @return Normalized Vector of the operand. Return zero-vector on zero-vector.
     */
    Vec<N, T> normalize_or_zero() const {
        Vec<N, T> temp{};
        const T length = this->length();
        if (length == 0)
            return Vec<N, T>{0, 0, 0};
        for (unsigned int i = 0; i < N; i++) {
            temp[i] = (*this)[i] / length;
        }
        return temp;
    }
    T angle(const Vec<N, T> other) const {
        return std::acos(this->dot(other) / (this->length() * other.length()));
    }

    /**
     * @brief Defined only for vec3.
     * @return Projection on target Vector.
     */
    Vec<3, T> project(const Vec<3, T> other) const requires (N==3) {
        return other * (this->dot(other) / other.length2());
    }

    /**
     * @brief Vector rotation based on Rodrigues' rotation formula.
     * @return Rotation on input axis by input radian.
     */
    Vec<3, T> rotate(const T &radian, const Vec<3, T> axis = {0, 0, 1}) const requires (N==3) {
        Vec<3, T> n = axis.normalize();

        return (1 - std::cos(radian)) * (n.dot((*this)) * (n)) +
               std::cos(radian) * (*this) + std::sin(radian) * (this->cross(n));
    }

    /***************************************
            Operators Overload
    ****************************************/

    /**
     * @brief Custom boolean casting for vector. This require the element in the Vec to be able to convert to bool.
     * @return True if and only if all vector elements are True.
     */
    explicit operator bool() const {
        for (unsigned int i = 0; i < N; i++) {
            if (!(*this)[i])
                return false;
        }
        return true;
    }
    /**
     * @brief Unary - operator, simply flip all element.
     */
    friend Vec<N, T> operator-(const Vec<N, T> &a) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = -a[i];
        }
        return result;
    }

    friend Vec<N, T> operator+(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a[i] + b[i];
        }
        return result;
    };
    friend Vec<N, T> operator-(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a[i] - b[i];
        }
        return result;
    }
    friend Vec<N, T> operator*(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a[i] * b[i];
        }
        return result;
    }
    friend Vec<N, T> operator*(const Vec<N, T> &a, const T &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a[i] * b;
        }
        return result;
    }
    friend Vec<N, T> operator*(const T &a, const Vec<N, T> &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a * b[i];
        }
        return result;
    }
    friend Vec<N, T> operator/(const Vec<N, T> &a, const T &b) {
        return a * (1 / b);
    }
    friend Vec<N, T> operator%(const Vec<N, T> &a, const T &b) {
        Vec<N, T> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = a[i] % b;
        }
        return result;
    }
    Vec<N, T> &operator+=(const Vec<N, T> &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] += other[i];
        }
        return *this;
    };
    Vec<N, T> &operator-=(const Vec<N, T> &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] -= other[i];
        }
        return *this;
    };
    Vec<N, T> &operator*=(const Vec<N, T> &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] *= other[i];
        }
        return *this;
    };
    Vec<N, T> &operator*=(const T &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] *= other;
        }
        return *this;
    };
    Vec<N, T> &operator/=(const Vec<N, T> &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] /= other[i];
        }
        return *this;
    };
    Vec<N, T> &operator/=(const T &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] /= other;
        }
        return *this;
    };
    Vec<N, T> &operator%=(const Vec<N, T> &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] %= other[i];
        }
        return *this;
    };
    Vec<N, T> &operator%=(const T &other) {
        for (unsigned int i = 0; i < N; i++) {
            (*this)[i] %= other;
        }
        return *this;
    };

    /***************************************
            Relational Operators
    ****************************************/
    friend Vec<N, unsigned int> operator==(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] == b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Vec<N, unsigned int> operator!=(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] != b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Vec<N, unsigned int> operator<(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] < b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Vec<N, unsigned int> operator>(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] > b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Vec<N, unsigned int> operator<=(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] <= b[i]) ? 1 : 0;
        }
        return result;
    }
    friend Vec<N, unsigned int> operator>=(const Vec<N, T> &a, const Vec<N, T> &b) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i] >= b[i]) ? 1 : 0;
        }
        return result;
    }
    friend bool operator!(const Vec<N, T> &a) {
        Vec<N, unsigned int> result{};
        for (unsigned int i = 0; i < N; i++) {
            result[i] = (a[i]) ? 1 : 0;
        }
        return result;
    }
    friend std::ostream &operator<<(std::ostream &o, const Vec<N, T> &vec) {
        o << "Vec" << N << "<";
        for (int i = 0; i < N; i++) {
            o << vec[i];
            if (i != N - 1) {
                o << ", ";
            }
        }
        o << ">";
        return o;
    }
};
using Vec2u = Vec<2, unsigned int>;
using Vec2f = Vec<2, float>;
using Vec2d = Vec<2, double>;
using Vec2b = Vec<2, bool>;
using Vec3u = Vec<3, unsigned int>;
using Vec3f = Vec<3, float>;
using Vec3d = Vec<3, double>;
using Vec3b = Vec<3, bool>;
using Vec4u = Vec<4, unsigned int>;
using Vec4f = Vec<4, float>;
using Vec4d = Vec<4, double>;
using Vec4b = Vec<4, bool>;
} // namespace smath
#endif // SMATH_VEC3_HPP
