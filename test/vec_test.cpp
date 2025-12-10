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
TEST(VEC_NORMALIZE_TEST) {
    assert_equal(Vec3f(0, 0, 0).normalise(), Vec3f(0, 0, 0)); // zero vector stays zero
    assert_close(Vec3f(3, 4, 0).normalise().length(), 1.0f, 0.0001f);
    assert_close(Vec3f(1, 1, 1).normalise().length(), 1.0f, 0.0001f);
}

TEST(VEC_DOT_PRODUCT_TEST) {
    assert_equal(Vec3f(1, 2, 3).dot(Vec3f(4, -5, 6)), 12.0f);
    assert_equal(Vec3f(1, 0, 0).dot(Vec3f(0, 1, 0)), 0.0f);
    assert_equal(Vec3f(-1, -1, -1).dot(Vec3f(1, 1, 1)), -3.0f);
}

TEST(VEC_CROSS_PRODUCT_PROPERTIES) {
    auto a = Vec3f(1, 0, 0);
    auto b = Vec3f(0, 1, 0);
    auto c = a.cross(b);
    assert_equal(c, Vec3f(0, 0, 1));
    assert_equal(a.dot(c), 0.0f);  // perpendicular
    assert_equal(b.dot(c), 0.0f);
    assert_equal(a.cross(a), Vec3f(0, 0, 0)); // self cross = zero
}

TEST(VEC_COMPARISON_OPERATORS) {
    assert_equal(Vec3f(1, 2, 3) == Vec3f(1, 2, 3), Vec3b(true, true, true));
    assert_equal(Vec3f(1, 2, 3) != Vec3f(1, 2, 4), Vec3b(false, false, true));
    assert_equal(Vec3f(1, 5, 3) > Vec3f(0, 4, 3), Vec3b(true, true, false));
    assert_equal(Vec3f(1, 2, 3) <= Vec3f(1, 3, 3), Vec3b(true, true, true));
}

TEST(VEC_SWIZZLING_LIKE_ACCESS) {
    Vec4f v(1, 2, 3, 4);
    assert_equal(v[0], 1.0f);
    assert_equal(v[3], 4.0f);
    // Note: no built-in swizzling, but we can test indexing safety
}

TEST(VEC_NEGATION_AND_ASSIGNMENT) {
    Vec3f v(1, -2, 3);
    assert_equal(-v, Vec3f(-1, 2, -3));
    v *= 2.0f;
    assert_equal(v, Vec3f(2, -4, 6));
    v /= 2.0f;
    assert_equal(v, Vec3f(1, -2, 3));
}

int main() { return TestRunner::instance().run("Length Test"); }
