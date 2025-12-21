#ifndef SMATH_COMMON_HPP
#define SMATH_COMMON_HPP

#include "smath/mat.hpp"
#include "smath/vec.hpp"
#include <concepts>
#include <type_traits>
#define PI 3.141592653589793d

// template <unsigned int N, typename T> class Vec;
// template <unsigned int M, unsigned int N, typename T> class Mat;
namespace smath {
template <class T>
    requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
constexpr T to_radian(T degree) {
    return static_cast<T>(degree * (PI / 180.0d));
}

template <class T>
    requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
constexpr T to_degree(T radian) {
    return static_cast<T>(radian / (PI / 180.0d));
}

/***************************************
       Clamp
***************************************/
template <class T>
constexpr T clamp(const T &lower, const T &target, const T &upper) {
    return (target < lower) ? lower : (target > upper) ? upper : target;
}
template <unsigned int N, class T>
constexpr Vec<N, T> clamp(const Vec<N, T> &lower, const Vec<N, T> &target,
                          const Vec<N, T> &upper) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
        result[i] = (target[i] < lower[i])   ? lower[i]
                    : (target[i] > upper[i]) ? upper[i]
                                             : target[i];
    }
    return result;
}
template <unsigned int N, class T>
constexpr Vec<N, T> clamp(const T &lower, const Vec<N, T> &target,
                          const T &upper) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
        result[i] = (target[i] < lower)   ? lower
                    : (target[i] > upper) ? upper
                                          : target[i];
    }
    return result;
}
template <unsigned int M, unsigned int N, class T>
constexpr Mat<M, N, T> clamp(const Mat<M, N, T> &lower,
                             const Mat<M, N, T> &target,
                             const Mat<M, N, T> &upper) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
        result[i] = (target[i] < lower[i])   ? lower[i]
                    : (target[i] > upper[i]) ? upper[i]
                                             : target[i];
    }
    return result;
}
template <unsigned int M, unsigned int N, class T>
constexpr Mat<M, N, T> clamp(const T &lower, const Mat<M, N, T> &target,
                             const T &upper) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
        result[i] = (target[i] < lower)   ? lower
                    : (target[i] > upper) ? upper
                                          : target[i];
    }
    return result;
}
/***************************************
        Saturate
***************************************/
template <class T> constexpr T saturate(const T &target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    return (target < lower) ? lower : (target > upper) ? upper : target;
}
template <unsigned int N, class T>
constexpr Vec<N, T> saturate(const Vec<N, T> &target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
        result[i] = (target[i] < lower)   ? lower
                    : (target[i] > upper) ? upper
                                          : target[i];
    }
    return result;
}
template <unsigned int M, unsigned int N, class T>
constexpr Mat<M, N, T> saturate(const Mat<M, N, T> &target) {
    const T lower = static_cast<T>(0.0f);
    const T upper = static_cast<T>(1.0f);
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < N * M; i++) {
        result[i] = (target[i] < lower)   ? lower
                    : (target[i] > upper) ? upper
                                          : target[i];
    }
    return result;
}

/***************************************
        Mix (Linear)
***************************************/

template <class T>
    requires std::is_arithmetic_v<T>
constexpr T mix(const T &a, const T &b, const float &mix) {
    return (b - a) * mix + a;
}

} // namespace smath
#endif
