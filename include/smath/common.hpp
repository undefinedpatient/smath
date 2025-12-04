#ifndef SMATH_BASIC_HPP
#define SMATH_BASIC_HPP

#include <concepts>
#include <type_traits>
#define PI 3.141592653589793

template<class T>
requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
T to_radian(double degree) {
    return degree*(PI/180);
}

template<class T>
requires std::is_arithmetic_v<T> && std::convertible_to<double, T>
T to_degree(double radian) {
    return radian/(PI/180);
}
#endif
