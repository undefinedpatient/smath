#ifndef SMATH_TEST_TOOL_HPP
#define SMATH_TEST_TOOL_HPP
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#define TEST(test_name) \
    void test_body_##test_name();\
    void (*ptest_body_##test_name)() = &test_body_##test_name;\
    bool registered_##test_name = [](){\
        TestRunner::instance().add_test(TestCase{#test_name, ptest_body_##test_name});\
        return true;\
    }();\
    void test_body_##test_name()

#define assert_equal(value, expected) assert_equal_impl(value, expected, #value)
#define assert_close(value, expected, threshold) assert_close_impl(value, expected, threshold, #value)

constexpr int MESSAGE_WIDTH = 64;
constexpr const char* passed_message = "\033[1;32m[PASSED]\033[0m";
constexpr const char* failed_message = "\033[1;31m[FAILED]\033[0m";

struct TestCase {
    const char* name;
    void (*body)();
};
static void print_message(const char& deco, const std::string &message) {
    const int pad = (MESSAGE_WIDTH - static_cast<int>(message.length()))/2;
    std::cout << "\033[1m+" << std::string(pad, deco) << " ";
    std::cout << message << " ";
    std::cout << std::string(pad, deco) << "+\033[0m" << std::endl;
}
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
        int run(const char* test_name){
            print_message('=', test_name);
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
            print_message('=', std::to_string(testCases.size()-num_failed) + "/" + std::to_string(this->testCases.size()) + " Test Passed");
            return num_failed ? EXIT_FAILURE : EXIT_SUCCESS;
        }

};

template<class T>
static void assert_equal_impl(const T& value, const T& expected, const char* valueString){
    std::cout<<"Test Type: Assert Equal \nEvaluate: "<<valueString<<"\nGet: "<< value <<"\nExpected: " << expected << "\n";
    if (value!=expected){
        std::cout<<failed_message<<"\n";
    }
    else
        std::cout<<passed_message<<"\n";
    std::cout<<std::endl;
    
}

template<class T>
static void assert_close_impl(const T& value, const T& expected, const T& threshold, const char* valueString){
    std::cout<<"Test Type: Assert Close \nEvaluate: "<<valueString<<"\n";
    std::cout<<"Get: "<< value <<"\n";
    std::cout<<"Expected: " << expected - threshold << " < " <<expected << " < " << expected + threshold << "\n";
    if (value>expected+threshold||value<expected-threshold){
        std::cout<<failed_message<<"\n";
    }
    else
        std::cout<<passed_message<<"\n";
    std::cout<<std::endl;
    
}
    
#endif //SMATH_TEST_TOOL_HPP
