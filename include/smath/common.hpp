#ifndef SMATH_BASIC_HPP
#define SMATH_BASIC_HPP

#include <concepts>
#include <type_traits>
#define PI 3.141592653589793d

template <class T>
    requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
T to_radian(T degree) {
    return static_cast(degree * (PI / 180.0d));
}

template <class T>
    requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
T to_degree(T radian) {
    return static_cast<T>(radian / (PI / 180.0d));
}


#endif
