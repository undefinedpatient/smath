#ifndef SMATH_VEC_HH
#define SMATH_VEC_HH

#include <cmath>
#include <concepts>
#include <cstring>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace smath {
template <unsigned int N, typename T>
  requires(std::is_arithmetic_v<T> && N <= 32)
class Vec {
private:
  T data[N];

public:
  // ==========================================================================================
  // Constructors
  // ==========================================================================================
  Vec() : data{} {};

  template <class... Us>
    requires(sizeof...(Us) == N && (std::convertible_to<Us, T> && ...))
  Vec(Us... args) : data{static_cast<T>(args)...} {}

  // Fill constructor: every component is set to value.
  Vec(T value) {
    for (unsigned int i = 0; i < N; i++) {
      data[i] = value;
    }
  }

  // Construct from a raw array of N elements.
  explicit Vec(const T *arr) { std::memcpy(this->data, arr, N * sizeof(T)); }

  // Copy constructor
  Vec(const Vec &other) = default;

  // Move constructor
  Vec(Vec &&other) noexcept = default;

  // Copy assignment
  Vec &operator=(const Vec<N, T> &other) = default;

  // Move assignment
  Vec &operator=(Vec<N, T> &&other) noexcept = default;

  // Initializer list
  Vec(std::initializer_list<T> values) {
    if (values.size() != N) {
      throw std::invalid_argument(
          "Number of values mismatched target vector size.");
    }
    std::memcpy(data, values.begin(), sizeof(T) * N);
  }

  // ==========================================================================================
  // Getters
  // ==========================================================================================

  // Access the element at zero-based index i.
  T &operator[](int i) {
    if (i < 0 || i > N - 1)
      throw std::out_of_range("Index out of bound");
    return data[i];
  }

  // Access the element at zero-based index i (const).
  const T &operator[](int i) const {
    if (i < 0 || i > N - 1)
      throw std::out_of_range("Index out of bound");
    return data[i];
  }

  // String representation of the vector.
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

#if defined(__cpp_multidimensional_subscript)
  // Swizzling-like accessor. Only available with C++23 multidimensional
  // subscript.
  auto operator[](const unsigned int index, auto... indices) const
    requires(sizeof...(indices) > 0)
  {
    return Vec<sizeof...(indices) + 1, T>{(*this)[index], (*this)[indices]...};
  }
#endif

  // ==========================================================================================
  // Operations
  // ==========================================================================================

  // Length of the vector.
  T length() const {
    T len = 0;
    for (unsigned int i = 0; i < N; i++) {
      len += data[i] * data[i];
    }
    return static_cast<T>(std::sqrt(len));
  }

  // Squared length of the vector.
  T length2() const {
    T len = 0;
    for (unsigned int i = 0; i < N; i++) {
      len += data[i] * data[i];
    }
    return len;
  }

  // Classic cross product. Defined only for 3-component vectors.
  Vec<3, T> cross(const Vec<3, T> &other) const
    requires(N == 3)
  {
    return Vec<3, T>{(*this)[1] * other[2] - (*this)[2] * other[1],
                     -(*this)[0] * other[2] + (*this)[2] * other[0],
                     (*this)[0] * other[1] - (*this)[1] * other[0]};
  }

  // Classic dot product.
  T dot(const Vec<N, T> &other) const {
    T total = 0;
    for (unsigned int i = 0; i < N; i++) {
      total += (*this)[i] * other[i];
    }
    return total;
  }

  // Append a single component, producing a vector of size N + 1.
  Vec<N + 1, T> expand(const T &value) const {
    Vec<N + 1, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (*this)[i];
    }
    result[N] = value;
    return result;
  }

  // Concatenate another vector, producing a vector of size N + M.
  template <unsigned int M>
  Vec<N + M, T> combine(const Vec<M, T> &other) const {
    Vec<N + M, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (*this)[i];
    }
    for (unsigned int i = 0; i < M; i++) {
      result[N + i] = other[i];
    }
    return result;
  }

  // Normalized vector. Raises when the operand is a zero-vector.
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

  // Normalized vector. Returns a zero-vector when the operand is a zero-vector.
  Vec<N, T> normalize_or_zero() const {
    const T length = this->length();
    if (length == 0)
      return Vec<N, T>{};
    Vec<N, T> temp{};
    for (unsigned int i = 0; i < N; i++) {
      temp[i] = (*this)[i] / length;
    }
    return temp;
  }

  // Angle between two vectors in radians.
  T angle(const Vec<N, T> &other) const {
    const T denominator = this->length() * other.length();
    if (denominator == 0)
      throw std::logic_error("Cannot compute an angle with a zero-vector.");
    T cosine = this->dot(other) / denominator;
    if (cosine > static_cast<T>(1))
      cosine = static_cast<T>(1);
    if (cosine < static_cast<T>(-1))
      cosine = static_cast<T>(-1);
    return std::acos(cosine);
  }

  // Projection onto a target vector. Defined only for 3-component vectors.
  Vec<3, T> project(const Vec<3, T> other) const
    requires(N == 3)
  {
    const T other_length2 = other.length2();
    if (other_length2 == 0)
      throw std::logic_error("Cannot project onto a zero-vector.");
    return other * (this->dot(other) / other_length2);
  }

  // Rotate around axis by radian using Rodrigues' rotation formula.
  Vec<3, T> rotate(const T &radian, const Vec<3, T> axis = {0, 0, 1}) const
    requires(N == 3)
  {
    Vec<3, T> n = axis.normalize();

    return (1 - std::cos(radian)) * (n.dot((*this)) * (n)) +
           std::cos(radian) * (*this) + std::sin(radian) * (this->cross(n));
  }

  bool all() const {
    for (unsigned int i = 0; i < N; i++) {
      if (!data[i])
        return false;
    }
    return true;
  }
  bool any() const {
    for (unsigned int i = 0; i < N; i++) {
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
    for (unsigned int i = 0; i < N; i++) {
      if (!(*this)[i])
        return false;
    }
    return true;
  }

  // Unary - operator, flips every component.
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
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = a[i] / b;
    }
    return result;
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

  // ==========================================================================================
  // Relational Operators
  // ==========================================================================================

  // Component-wise comparison, returning 0/1 per component.
  friend Vec<N, unsigned int> operator==(const Vec<N, T> &a,
                                         const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] == b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Vec<N, unsigned int> operator!=(const Vec<N, T> &a,
                                         const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] != b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Vec<N, unsigned int> operator<(const Vec<N, T> &a,
                                        const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] < b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Vec<N, unsigned int> operator>(const Vec<N, T> &a,
                                        const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] > b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Vec<N, unsigned int> operator<=(const Vec<N, T> &a,
                                         const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] <= b[i]) ? 1 : 0;
    }
    return result;
  }
  friend Vec<N, unsigned int> operator>=(const Vec<N, T> &a,
                                         const Vec<N, T> &b) {
    Vec<N, unsigned int> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (a[i] >= b[i]) ? 1 : 0;
    }
    return result;
  }

  // Logical negation: true unless every component is truthy.
  friend bool operator!(const Vec<N, T> &a) { return !a.all(); }

  friend std::ostream &operator<<(std::ostream &o, const Vec<N, T> &vec) {
    o << "Vec" << N << "<";
    for (unsigned int i = 0; i < N; i++) {
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
#endif // SMATH_VEC_HH
