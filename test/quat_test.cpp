#include "test_tool.hpp"
#include "smath.hpp"

using namespace smath;
TEST(PRINT_TEST) {
    std::cout<< Quat<float>{1,2,3,4}.to_string() << std::endl;
}

int main() { return TestRunner::instance().run("Quaternion Test"); }