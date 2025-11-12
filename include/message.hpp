#ifndef SMATH_MESSAGE_H
#define SMATH_MESSAGE_H
#include <iostream>
#include <string>

constexpr int MESSAGE_WIDTH = 64;
constexpr const char* passed_message = "\033[1;32m[PASSED]\033[0m";
constexpr const char* failed_message = "\033[1;31m[FAILED]\033[0m";
inline void print_message(const char& deco, const std::string &message) {
    const int pad = (MESSAGE_WIDTH - static_cast<int>(message.length()))/2;
    std::cout << "\033[1m+" << std::string(pad, deco) << " ";
    std::cout << message << " ";
    std::cout << std::string(pad, deco) << "+\033[0m" << std::endl;
}


template<class T>
void assert_equal_impl(const T& value, const T& expected, const char* valueString){
    std::cout<<"Test: AssertEqual \nEvaluate: "<<valueString<<"\nGet: "<< value <<"\nExpected: " << expected << "\n";
    if (value!=expected){
        std::cout<<failed_message<<"\n";
    }
    else
        std::cout<<passed_message<<"\n";
    std::cout<<std::endl;
}
#define assert_equal(value, expected) assert_equal_impl(value, expected, #value)
#endif //SMATH_MESSAGE_H
