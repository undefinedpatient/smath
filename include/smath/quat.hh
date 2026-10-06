#ifndef SMATH_QUAT_HH
#define SMATH_QUAT_HH

#include "mat.hh"
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <string>

namespace smath {
template <typename T>
  requires(std::is_arithmetic_v<T>)
class Quat {
private:
  T data[4];

public:
  // ==========================================================================================
  // Constructors
  // ==========================================================================================
  Quat() : data{} {}
  Quat(const T &q0, const T &q1, const T &q2, const T &q3)
      : data{q0, q1, q2, q3} {}
  Quat(const T &real, const Vec<3, T> &imaginary)
      : data{real, imaginary[0], imaginary[1], imaginary[2]} {}
  // Copy constructor
  Quat(const Quat &other) = default;
  // Move constructor
  Quat(Quat &&other) noexcept = default;
  // Copy assignment
  Quat &operator=(const Quat &other) = default;
  // Move assignment
  Quat &operator=(Quat &&other) noexcept = default;
  // Initializer list
  Quat(std::initializer_list<T> values) {
    if (values.size() != 4) {
      throw std::invalid_argument(
          "Number of values mismatched quaternion size.");
    }
    std::memcpy(data, values.begin(), sizeof(T) * 4);
  }

  // Identity (no-rotation) quaternion.
  static Quat<T> identity() { return Quat<T>{1.0f, 0.0f, 0.0f, 0.0f}; }

  // ==========================================================================================
  // Getters
  // ==========================================================================================

  // Access the component at zero-based index i.
  T &operator[](int i) {
    if (i < 0 || i > 3)
      throw std::out_of_range("Index out of bound");
    return data[i];
  }
  const T &operator[](int i) const {
    if (i < 0 || i > 3)
      throw std::out_of_range("Index out of bound");
    return data[i];
  }

  // Scalar (real) component.
  T scalar() const { return data[0]; }

  // Imaginary (vector) component.
  Vec<3, T> vector() const { return Vec<3, T>{data[1], data[2], data[3]}; }

  // String representation of the quaternion.
  constexpr std::string to_string() const {
    std::string str = "Quat (";
    for (unsigned int i = 0; i < 4; i++) {
      str += std::to_string(data[i]);
      switch (i) {
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

  // ==========================================================================================
  // Operations
  // ==========================================================================================

  // Hamilton product of two quaternions.
  Quat<T> mul(const Quat<T> &other) const {
    return Quat<T>{data[0] * other[0] - data[1] * other[1] -
                       data[2] * other[2] - data[3] * other[3],
                   data[0] * other[1] + data[1] * other[0] +
                       data[2] * other[3] - data[3] * other[2],
                   data[0] * other[2] + data[2] * other[0] +
                       data[3] * other[1] - data[1] * other[3],
                   data[0] * other[3] + data[3] * other[0] +
                       data[1] * other[2] - data[2] * other[1]};
  }

  T dot(const Quat<T> &other) const {
    return data[0] * other[0] + data[1] * other[1] + data[2] * other[2] +
           data[3] * other[3];
  }

  Quat<T> conjugate() const {
    return Quat{data[0], -data[1], -data[2], -data[3]};
  }

  // Normalized quaternion. Raises when the operand is a zero-quaternion.
  Quat<T> normalize() const {
    T divisor = length();
    if (divisor == 0) {
      throw std::logic_error("Cannot normalize a zero-quaternion.");
    }
    return (*this) / divisor;
  }

  // Normalized quaternion. Returns a zero-quaternion for a zero-quaternion.
  Quat<T> normalize_or_zero() const {
    T divisor = length();
    if (divisor == 0) {
      return {0, 0, 0, 0};
    }
    return (*this) / divisor;
  }

  // Normalized quaternion. Returns the identity quaternion for a
  // zero-quaternion.
  Quat<T> normalize_or_one() const {
    T divisor = length();
    if (divisor == 0) {
      return {1, 0, 0, 0};
    }
    return (*this) / divisor;
  }

  // Length of the quaternion.
  T length() const { return std::sqrt(length2()); }

  // Squared length of the quaternion.
  T length2() const {
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2] +
           data[3] * data[3];
  }

  Quat<T> inverse() const { return this->conjugate() / this->length2(); }

  // Convert the quaternion into a 3x3 rotation matrix.
  Mat<3, 3, T> to_mat3() const {
    auto q = this->normalize_or_one();
    return Mat<3, 3, T>{static_cast<T>(2.0f) *
                            (q[0] * q[0] + q[1] * q[1] - static_cast<T>(0.5)),
                        static_cast<T>(2.0f) * (q[0] * q[3] + q[1] * q[2]),
                        static_cast<T>(2.0f) * (q[1] * q[3] - q[0] * q[2]),
                        static_cast<T>(2.0f) * (q[1] * q[2] - q[0] * q[3]),
                        static_cast<T>(2.0f) *
                            (q[0] * q[0] + q[2] * q[2] - static_cast<T>(0.5)),
                        static_cast<T>(2.0f) * (q[0] * q[1] + q[2] * q[3]),
                        static_cast<T>(2.0f) * (q[0] * q[2] + q[1] * q[3]),
                        static_cast<T>(2.0f) * (q[2] * q[3] - q[0] * q[1]),
                        static_cast<T>(2.0f) *
                            (q[0] * q[0] + q[3] * q[3] - static_cast<T>(0.5))};
  }

  // Convert the quaternion into a 4x4 homogeneous rotation matrix.
  Mat<4, 4, T> to_mat4() const { return to_mat3().to_homogeneous(); }

  // Convert a 3x3 rotation matrix into a quaternion (Shepperd's method).
  static Quat<T> from_mat3(const Mat<3, 3, T> &m) {
    const T trace = m[0] + m[4] + m[8];
    if (trace > static_cast<T>(0)) {
      const T s = std::sqrt(trace + static_cast<T>(1)) * static_cast<T>(2);
      return Quat<T>{s / static_cast<T>(4), (m[5] - m[7]) / s,
                     (m[6] - m[2]) / s, (m[1] - m[3]) / s};
    } else if (m[0] > m[4] && m[0] > m[8]) {
      const T s =
          std::sqrt(static_cast<T>(1) + m[0] - m[4] - m[8]) * static_cast<T>(2);
      return Quat<T>{(m[5] - m[7]) / s, s / static_cast<T>(4),
                     (m[1] + m[3]) / s, (m[2] + m[6]) / s};
    } else if (m[4] > m[8]) {
      const T s =
          std::sqrt(static_cast<T>(1) + m[4] - m[0] - m[8]) * static_cast<T>(2);
      return Quat<T>{(m[6] - m[2]) / s, (m[1] + m[3]) / s,
                     s / static_cast<T>(4), (m[5] + m[7]) / s};
    } else {
      const T s =
          std::sqrt(static_cast<T>(1) + m[8] - m[0] - m[4]) * static_cast<T>(2);
      return Quat<T>{(m[1] - m[3]) / s, (m[2] + m[6]) / s, (m[5] + m[7]) / s,
                     s / static_cast<T>(4)};
    }
  }

  // Convert a 4x4 rotation matrix into a quaternion.
  static Quat<T> from_mat4(const Mat<4, 4, T> &mat) {
    return from_mat3(mat.to_mat3());
  }

  bool all() const {
    for (unsigned int i = 0; i < 4; i++) {
      if (!data[i])
        return false;
    }
    return true;
  }
  bool any() const {
    for (unsigned int i = 0; i < 4; i++) {
      if (data[i])
        return true;
    }
    return false;
  }
  bool none() const { return !any(); }

  // ==========================================================================================
  // Operators
  // ==========================================================================================

  // Boolean cast. True if and only if every component is truthy.
  explicit operator bool() const {
    for (unsigned int i = 0; i < 4; i++) {
      if (!(*this)[i])
        return false;
    }
    return true;
  }

  // Unary - operator, flips every component.
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
    Quat<T> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = a[i] / b;
    }
    return result;
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

  // ==========================================================================================
  // Relational Operators
  // ==========================================================================================

  // Component-wise comparison, returning 0/1 per component.
  friend Quat<unsigned int> operator==(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] == b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator!=(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] != b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator<(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] < b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator>(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] > b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator<=(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] <= b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator>=(const Quat<T> &a, const Quat<T> &b) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i] >= b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Quat<unsigned int> operator!(const Quat<T> &a) {
    Quat<unsigned int> result{};
    for (unsigned int i = 0; i < 4; i++) {
      result[i] = (a[i]) ? 1 : 0;
    }
    return result;
  }
  friend std::ostream &operator<<(std::ostream &o, const Quat<T> &quat) {
    o << quat.to_string();
    return o;
  }
};

// Spherical linear interpolation between two quaternions.
template <typename T>
Quat<T> slerp(const Quat<T> &a, const Quat<T> &b, const T &t) {
  Quat<T> a_n = a.normalize_or_one();
  Quat<T> b_n = b.normalize_or_one();
  T dot = a_n.dot(b_n);
  // Take the shortest path.
  if (dot < static_cast<T>(0)) {
    b_n = -b_n;
    dot = -dot;
  }
  // Fall back to linear interpolation when the inputs are nearly parallel.
  if (dot > static_cast<T>(0.9995)) {
    return (a_n * (static_cast<T>(1) - t) + b_n * t).normalize_or_one();
  }
  const T angle = std::acos(dot);
  const T sin_angle = std::sin(angle);
  return a_n * (std::sin((static_cast<T>(1) - t) * angle) / sin_angle) +
         b_n * (std::sin(t * angle) / sin_angle);
}
} // namespace smath
#endif // SMATH_QUAT_HH
