#ifndef SMATH_COMMON_HH
#define SMATH_COMMON_HH

#include "vec.hh"
#include "mat.hh"
#include <concepts>
#include <stdexcept>
#include <type_traits>

namespace smath {
  inline constexpr double PI = 3.14159265358979323846;

  // Convert degrees to radians.
  template <class T>
    requires std::is_arithmetic_v<T>&& std::convertible_to<double, T>
  constexpr T to_radian(T degree) {
    return static_cast<T>(degree * (PI / 180.0));
  }

  // Convert radians to degrees.
  template <class T>
    requires std::is_arithmetic_v<T>&& std::convertible_to<double, T>
  constexpr T to_degree(T radian) {
    return static_cast<T>(radian / (PI / 180.0));
  }

  // ============================================================================================
  // Clamp
  // ============================================================================================
  template <class T>
  constexpr T clamp(const T& target, const T& lower, const T& upper) {
    return (target < lower) ? lower : (target > upper) ? upper : target;
  }
  template <unsigned int N, class T>
  constexpr Vec<N, T> clamp(const Vec<N, T>& target, const Vec<N, T>& lower,
    const Vec<N, T>& upper) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (target[i] < lower[i]) ? lower[i]
        : (target[i] > upper[i]) ? upper[i]
        : target[i];
    }
    return result;
  }
  template <unsigned int N, class T>
  constexpr Vec<N, T> clamp(const Vec<N, T>& target, const T& lower,
    const T& upper) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (target[i] < lower) ? lower
        : (target[i] > upper) ? upper
        : target[i];
    }
    return result;
  }
  template <unsigned int M, unsigned int N, class T>
  constexpr Mat<M, N, T> clamp(const Mat<M, N, T>& target,
    const Mat<M, N, T>& lower,
    const Mat<M, N, T>& upper) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
      result[i] = (target[i] < lower[i]) ? lower[i]
        : (target[i] > upper[i]) ? upper[i]
        : target[i];
    }
    return result;
  }
  template <unsigned int M, unsigned int N, class T>
  constexpr Mat<M, N, T> clamp(const Mat<M, N, T>& target, const T& lower,
    const T& upper) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
      result[i] = (target[i] < lower) ? lower
        : (target[i] > upper) ? upper
        : target[i];
    }
    return result;
  }

  // ============================================================================================
  // Saturate
  // ============================================================================================
  template <class T> constexpr T saturate(const T& target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    return (target < lower) ? lower : (target > upper) ? upper : target;
  }
  template <unsigned int N, class T>
  constexpr Vec<N, T> saturate(const Vec<N, T>& target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (target[i] < lower) ? lower
        : (target[i] > upper) ? upper
        : target[i];
    }
    return result;
  }
  template <unsigned int M, unsigned int N, class T>
  constexpr Mat<M, N, T> saturate(const Mat<M, N, T>& target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
      result[i] = (target[i] < lower) ? lower
        : (target[i] > upper) ? upper
        : target[i];
    }
    return result;
  }

  // ============================================================================================
  // Mix (Linear)
  // ============================================================================================
  template <class T>
    requires std::is_arithmetic_v<T>
  constexpr T mix(const T& a, const T& b, const float& mix) {
    return (b - a) * mix + a;
  }

  template <unsigned int N, class T>
  constexpr Vec<N, T> mix(const Vec<N, T>& a, const Vec<N, T>& b,
    const float& mix) {
    return (b - a) * mix + a;
  }

  template <unsigned int M, unsigned int N, class T>
  constexpr Mat<M, N, T> mix(const Mat<M, N, T>& a, const Mat<M, N, T>& b,
    const float& mix) {
    return (b - a) * mix + a;
  }

  // ============================================================================================
  // Step
  // ============================================================================================
  template <class T>
    requires std::is_arithmetic_v<T>
  constexpr T step(const T& value, const T& threshold) {
    return (value > threshold) ? static_cast<T>(1) : static_cast<T>(0);
  }
  template <unsigned int N, class T>
  constexpr Vec<N, T> step(const Vec<N, T>& value, const T& threshold) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
      result[i] = (value[i] > threshold) ? static_cast<T>(1) : static_cast<T>(0);
    }
    return result;
  }
  template <unsigned int M, unsigned int N, class T>
  constexpr Mat<M, N, T> step(const Mat<M, N, T>& value,
    const T& threshold) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < M * N; i++) {
      result[i] = (value[i] > threshold) ? static_cast<T>(1) : static_cast<T>(0);
    }
    return result;
  }

  // Smooth Hermite interpolation between 0 and 1.
  template <class T>
    requires std::is_arithmetic_v<T>
  constexpr T smooth_step(const T& low, const T& high, const T& value) {
    auto v = clamp((value - low) / (high - low), static_cast<T>(0), static_cast<T>(1));
    return static_cast<T>(v * v * (static_cast<T>(3) - static_cast<T>(2) * v));
  }
  template <unsigned int N, class T>
    requires std::is_arithmetic_v<T>
  constexpr Vec<N, T> smooth_step(const T& low, const T& high, const Vec<N, T>& value) {
    const Vec<N, T> v = clamp((value - Vec<N, T>(low)) / (high - low),
      static_cast<T>(0), static_cast<T>(1));
    return v * v * (Vec<N, T>(static_cast<T>(3)) - Vec<N, T>(static_cast<T>(2)) * v);
  }
  template <unsigned int M, unsigned int N, class T>
    requires std::is_arithmetic_v<T>
  constexpr Mat<M, N, T> smooth_step(const T& low, const T& high, const Mat<M, N, T>& value) {
    const Mat<M, N, T> v = clamp((value - Mat<M, N, T>(low)) / (high - low),
      static_cast<T>(0), static_cast<T>(1));
    return v * v * (Mat<M, N, T>(static_cast<T>(3)) - Mat<M, N, T>(static_cast<T>(2)) * v);
  }

  // ============================================================================================
  // Reflect + Refract
  // ============================================================================================
  template<class T>
  Vec<3, T> reflect(const Vec<3, T>& incident, const Vec<3, T>& normal) {
    return incident - static_cast<T>(2) * normal.dot(incident) * normal;
  }

  // Refract incident around normal given the ratio r = n_0 / n_1. Returns a
  // zero-vector on total internal reflection.
  template<class T>
  Vec<3, T> refract(const Vec<3, T>& incident, const Vec<3, T>& normal, const T& r) {
    const T cos_i = incident.dot(normal);
    const T k = static_cast<T>(1) - r * r * (static_cast<T>(1) - cos_i * cos_i);
    if (k < static_cast<T>(0))
      return Vec<3, T>{};
    return r * incident - (r * cos_i + std::sqrt(k)) * normal;
  }

  template<class T>
  Vec<3, T> refract(const Vec<3, T>& incident, const Vec<3, T>& normal, const T& n_0, const T& n_1) {
    return refract(incident, normal, n_0 / n_1);
  }

  // ============================================================================================
  // Absolute
  // ============================================================================================
  template<unsigned int N, class T>
    requires (std::is_arithmetic_v<T>)
  Vec<N, T> absolute(const Vec<N, T>& vec) {
    Vec<N, T> result(vec);
    for (unsigned int i = 0; i < N; i++) {
      result[i] = static_cast<T>(std::abs(result[i]));
    }
    return result;
  }
  template<unsigned int M, unsigned int N, class T>
    requires (std::is_arithmetic_v<T>)
  Mat<M, N, T> absolute(const Mat<M, N, T>& mat) {
    Mat<M, N, T> result(mat);
    for (unsigned int i = 0; i < M * N; i++) {
      result[i] = static_cast<T>(std::abs(result[i]));
    }
    return result;
  }

  // ============================================================================================
  // Minkowski Distance
  // ============================================================================================
  template<unsigned int N, class T>
    requires (std::is_convertible_v<T, float>&& std::is_arithmetic_v<T>)
  T distance(const Vec<N, T>& a, const Vec<N, T>& b, const float& dimension) {
    if (dimension == 0) {
      throw std::invalid_argument("Dimension must be non-zero value.");
    }
    double temp = 0.0;
    for (unsigned int i = 0; i < N; i++) {
      temp += std::pow(std::abs(a[i] - b[i]), dimension);
    }
    return static_cast<T>(std::pow(temp, 1 / dimension));
  }
} // namespace smath
#endif // SMATH_COMMON_HH
