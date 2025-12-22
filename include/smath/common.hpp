#ifndef SMATH_COMMON_HPP
#define SMATH_COMMON_HPP

#include "quat.hpp"
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
constexpr T clamp(const T &target, const T &lower, const T &upper) {
    return (target < lower) ? lower : (target > upper) ? upper : target;
}
template <unsigned int N, class T>
constexpr Vec<N, T> clamp(const Vec<N, T> &target, const Vec<N, T> &lower,
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
constexpr Vec<N, T> clamp(const Vec<N, T> &target, const T &lower,
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
constexpr Mat<M, N, T> clamp(const Mat<M, N, T> &target,
                             const Mat<M, N, T> &lower,
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
constexpr Mat<M, N, T> clamp(const Mat<M, N, T> &target, const T &lower,
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

template <unsigned int N, class T>
constexpr Vec<N, T> mix(const Vec<N, T> &a, const Vec<N, T> &b,
                        const float &mix) {
    return (b - a) * mix + a;
}

template <unsigned int M, unsigned int N, class T>
constexpr Mat<M, N, T> mix(const Mat<M, N, T> &a, const Mat<M, N, T> &b,
                           const float &mix) {
    return (b - a) * mix + a;
}

/***************************************
        Step
***************************************/
template <class T>
    requires std::is_arithmetic_v<T>
constexpr T step(const T &value, const T &threshold) {
    return (value > threshold) ? static_cast<T>(1) : static_cast<T>(0);
}
template <unsigned int N, class T>
constexpr Vec<N, T> step(const Vec<N, T> &value, const T &threshold) {
    Vec<N, T> result{};
    for (unsigned int i = 0; i < N; i++) {
        result = (value[i] > threshold) ? static_cast<T>(1.0f)
                                        : static_cast<T>(0.0f);
    }
    return result;
}
template <unsigned int M, unsigned int N, class T>
constexpr Mat<M, N, T> step(const Mat<M, N, T> &value,
                                   const T &threshold) {
    Mat<M, N, T> result{};
    for (unsigned int i = 0; i < M * N; i++) {
        result = (value[i] > threshold) ? static_cast<T>(1.0f)
                                        : static_cast<T>(0.0f);
    }
    return result;
}
template <class T>
    requires std::is_arithmetic_v<T>
constexpr T smooth_step(const T &low, const T &high, const T &value) {
    auto v = clamp((value-low)/(high-low), 0.0f, 1.0f);
    return static_cast<T>(v*v*(3.0f-2.0f*v));
}
template <unsigned int N, class T>
    requires std::is_arithmetic_v<T>
constexpr Vec<N,T> smooth_step(const T &low, const T &high, const Vec<N,T> &value) {
    auto v = clamp((value-low)/(high-low), 0.0f, 1.0f);
    return v*v*(3.0f-2.0f*v);
}
template <unsigned int M, unsigned int N, class T>
    requires std::is_arithmetic_v<T>
constexpr Mat<M,N,T> smooth_step(const T &low, const T &high, const Mat<M,N,T> &value) {
    auto v = clamp((value-low)/(high-low), 0.0f, 1.0f);
    return v*v*(3.0f-2.0f*v);
}
} // namespace smath
#endif
