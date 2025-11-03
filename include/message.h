//
// Created by undef on 03/11/2025.
//

#ifndef SMATH_MESSAGE_H
#define SMATH_MESSAGE_H
#include <iostream>
#include <string>
constexpr int MESSAGE_WIDTH = 64;
inline void print_message(const char& deco, const std::string &message) {
    const int pad = (MESSAGE_WIDTH - static_cast<int>(message.length()))/2;
    std::cout << "+" << std::string(pad, deco) << " ";
    std::cout << message << " ";
    std::cout << std::string(pad, deco) << "+" << std::endl;
}
#endif //SMATH_MESSAGE_H