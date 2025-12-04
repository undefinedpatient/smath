#ifndef SMATH_TEST_TOOL_HPP
#define SMATH_TEST_TOOL_HPP
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ratio>
#include <smath/smath.hpp>
#include <string>
#include <vector>

#define TEST(test_name)                                                        \
    void test_body_##test_name();                                              \
    void (*ptest_body_##test_name)() = &test_body_##test_name;                 \
    bool registered_##test_name = []() {                                       \
        TestRunner::instance().add_test(                                       \
            TestCase{#test_name, ptest_body_##test_name});                     \
        return true;                                                           \
    }();                                                                       \
    void test_body_##test_name()

#define assert_equal(value, expected) assert_equal_impl(value, expected, #value)

#define assert_close(value, expected, threshold)                               \
    assert_close_impl(value, expected, threshold, #value)

constexpr int MESSAGE_WIDTH = 42;
constexpr const char *passed_message = "\033[1;32m[PASSED]\033[0m";
constexpr const char *failed_message = "\033[1;31m[FAILED]\033[0m";

static void print_message(const char &deco, const std::string &message) {
    const int pad = (MESSAGE_WIDTH - static_cast<int>(message.length())) / 2;
    std::cout << "\033[1m+" << std::string(pad, deco) << " ";
    std::cout << message << " ";
    std::cout << std::string(pad, deco) << "+\033[0m" << std::endl;
}
struct TestCase {
    const char *name;
    void (*body)();
};
class Timer {
  private:
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> stop_time;

  public:
    Timer() = default;
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    void stop() { stop_time = std::chrono::high_resolution_clock::now(); }
    const long get_duration() const {
        auto start_in_milli =
            std::chrono::time_point_cast<std::chrono::microseconds>(start_time)
                .time_since_epoch()
                .count();
        auto end_in_milli =
            std::chrono::time_point_cast<std::chrono::microseconds>(stop_time)
                .time_since_epoch()
                .count();
        long duration = end_in_milli - start_in_milli;
        return duration;
    }
};
class TestRunner {
  private:
    std::vector<TestCase> test_cases = {};

  public:
    static TestRunner &instance() {
        static TestRunner instance;
        return instance;
    }
    void add_test(const TestCase &test_case) {
        test_cases.push_back(test_case);
    }
    int run(const char *test_name) {
        print_message('=', test_name);
        int num_failed = 0;
        long time_count = 0;
        for (const TestCase &test : this->test_cases) {
            print_message('-', test.name);
            Timer timer{};
            try {
                timer.start();
                test.body();
                timer.stop();
                time_count += timer.get_duration();
            } catch (const std::exception &e) {
                num_failed++;
            } catch (...) {
                num_failed++;
            }
        }
        print_message('=', std::to_string(test_cases.size() - num_failed) +
                               "/" + std::to_string(this->test_cases.size()) +
                               " Passed | " + std::to_string(time_count) +
                               " \u00B5s ");
        return num_failed ? EXIT_FAILURE : EXIT_SUCCESS;
    }
};

template <class T>
static void assert_equal_impl(const T &value, const T &expected,
                              const char *valueString) {
    // PASSED
    if (value == expected) {
        std::cout << "Test: Assert Equal | Evaluate: " << valueString << "\n";
        std::cout << passed_message << "\n";
    }
    // FAILED
    else if (value != expected) {
        std::cout << "Test: Assert Equal \nEvaluate: " << valueString
                  << "\nGet:\n"
                  << value << "\nExpected:\n"
                  << expected << "\n";
        std::cout << failed_message << "\n";
    }
    std::cout << std::endl;
}

template <class T>
static void assert_close_impl(const T &value, const T &expected,
                              const T &threshold, const char *valueString) {
    // FAILED
    if (value > expected + threshold || value < expected - threshold) {
        std::cout << "Test: Assert Close \nEvaluate: " << valueString << "\n";
        std::cout << "Get:\n" << value << "\n";
        std::cout << "Expected:\n"
                  << expected - threshold << " < " << expected << " < "
                  << expected + threshold << "\n";
        std::cout << failed_message << "\n";
    }
    // PASSED
    else {
        std::cout << "Test: Assert Close | Evaluate: " << valueString << "\n";
        std::cout << passed_message << "\n";
    }
    std::cout << std::endl;
}

#endif // SMATH_TEST_TOOL_HPP
