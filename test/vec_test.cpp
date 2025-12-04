#include "test_tool.hpp"
#include <smath/smath.hpp>

using namespace smath;
TEST(LENGTH_TEST) {
    assert_equal(Vec3f(0, 0, 0).length(), 0.0f);
    assert_equal(Vec3f(4, 4, 2).length(), 6.0f);
    assert_equal(Vec3f(5, 0, 0).length(), 5.0f);
    assert_close(Vec3f(4, 2, 1).length(), 4.5825f, 0.001f);
};

TEST(ARITHMETHIC_TEST) {
    assert_equal(Vec3f(1.0f, 4.0f, -5.0f) + Vec3f(-2.0f, 1.0f, 0.0f),
                 Vec3f(-1.0f, 5.0f, -5.0f));
    assert_equal(Vec3f(1.0f, 4.0f, -5.0f) - Vec3f(-2.0f, 1.0f, 0.0f),
                 Vec3f(3.0f, 3.0f, -5.0f));
    assert_equal(Vec3f(1.0f, 4.0f, -5.0f) * -2.0f, Vec3f(-2.0f, -8.0f, 10.0f));
    assert_equal(Vec3f(1.0f, 4.0f, -5.0f) * 2.0f, Vec3f(2.0f, 8.0f, -10.0f));
    auto v00 = Vec3f(1.0f, 2.0f, 3.0f);
    auto v01 = Vec3f(-1.0f, 2.0f, 0.0f);
    auto r0 = v00.cross(v01);
    assert_equal(r0, Vec3f(-6, -3, 4));
}
int main() { return TestRunner::instance().run("Length Test"); }
