#include <smath/smath.hpp>
#include "test_tool.hpp"

using namespace smath;

TEST(LENGTH_TEST_0){
    assert_equal(Vec3f(0, 0, 0).length(), 0.0f);
};

TEST(LENGTH_TEST_1){
    assert_equal(Vec3f(4, 4, 2).length(), 6.0f);
};

TEST(LENGTH_TEST_2){
    assert_equal(Vec3f(5, 0, 0).length(), 5.0f);
};

TEST(LENGTH_TEST_3){
    assert_close(Vec3f(4,2,1).length(), 4.5825f, 0.001f);
};

int main(){
    return TestRunner::instance().run("Length Test");
}
