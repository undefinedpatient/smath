#ifndef SMATH_TEST_TOOL_HPP
#define SMATH_TEST_TOOL_HPP
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#define TEST(test_name) \
    

constexpr int MESSAGE_WIDTH = 64;
constexpr const char* passed_message = "\033[1;32m[PASSED]\033[0m";
constexpr const char* failed_message = "\033[1;31m[FAILED]\033[0m";

struct TestCase {
    char* name;
    void (*body)();
};
class TestRunner{
    private:
        std::vector<TestCase> testCases = {};
    public:
        static TestRunner& instance() {
            static TestRunner instance;
            return instance;
        }
        void add_test(const TestCase& test_case) {
            testCases.push_back(test_case);
        }
        int run(){
            int num_failed = 0;
            for(const TestCase& test: this->testCases){
                try {
                    test.body();
                }
                catch (const std::exception& e){
                    num_failed++;
                }
                catch (...){
                    num_failed++;
                }
            }
            return num_failed ? EXIT_SUCCESS : EXIT_FAILURE;
        }

};
static void print_message(const char& deco, const std::string &message) {
    const int pad = (MESSAGE_WIDTH - static_cast<int>(message.length()))/2;
    std::cout << "\033[1m+" << std::string(pad, deco) << " ";
    std::cout << message << " ";
    std::cout << std::string(pad, deco) << "+\033[0m" << std::endl;
}

#define assert_equal(value, expected) assert_equal_impl(value, expected, #value)
template<class T>
static void assert_equal_impl(const T& value, const T& expected, const char* valueString){
    std::cout<<"Test: AssertEqual \nEvaluate: "<<valueString<<"\nGet: "<< value <<"\nExpected: " << expected << "\n";
    if (value!=expected){
        std::cout<<failed_message<<"\n";
    }
    else
        std::cout<<passed_message<<"\n";
    std::cout<<std::endl;
    
}

    
#endif //SMATH_TEST_TOOL_HPP
