#include "test_tool.hpp"
#include "smath.hpp"

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
}

TEST(VEC_SWIZZLING_TEST){
    auto a0 = Vec4f(0,1,2,3)[0,1,2];
    auto b0 = Vec3f(0,1,2);
    auto a1 = Vec4f(0,1,2,3)[0,1,2,2,3];
    auto b1 = Vec<5,float>(0,1,2,2,3);
    auto a2 = Vec4f(0,1,2,3)[3,2,2,0];
    auto b2 = Vec4f(3,2,2,0);
    assert_equal(a0,b0);
    assert_equal(a1,b1);
    assert_equal(a2,b2);
}
TEST(VEC_NORMALIZE_TEST) {
    assert_equal(Vec3f(0, 0, 0).normalize_or_zero(), Vec3f(0, 0, 0)); // zero vector stays zero
    assert_close(Vec3f(3, 4, 0).normalize_or_zero().length(), 1.0f, 0.0001f);
    assert_close(Vec3f(1, 1, 1).normalize_or_zero().length(), 1.0f, 0.0001f);
}

TEST(VEC_DOT_PRODUCT_TEST) {
    assert_equal(Vec3f(1, 2, 3).dot(Vec3f(4, -5, 6)), 12.0f);
    assert_equal(Vec3f(1, 0, 0).dot(Vec3f(0, 1, 0)), 0.0f);
    assert_equal(Vec3f(-1, -1, -1).dot(Vec3f(1, 1, 1)), -3.0f);
}

TEST(VEC_CROSS_PRODUCT_TEST) {
    auto a = Vec3f(1, 0, 0);
    auto b = Vec3f(0, 1, 0);
    auto c = a.cross(b);
    assert_equal(c, Vec3f(0, 0, 1));
    assert_equal(a.dot(c), 0.0f);  // perpendicular
    assert_equal(b.dot(c), 0.0f);
    assert_equal(a.cross(a), Vec3f(0, 0, 0)); // self cross = zero

    auto a00 = Vec3f(1.0f, 2.0f, 3.0f);
    auto b00 = Vec3f(-1.0f, 2.0f, 0.0f);
    auto r00 = a00.cross(b00);
    assert_equal(r00, Vec3f(-6, -3, 4));

    auto a01 = Vec3f{7,0,2};
    auto b01 = Vec3f{1/std::sqrt(2),1/std::sqrt(2),0};
    auto r01 = a01.cross(b01);
    std::cout<<r01;
    assert_close(r01,Vec3f(-1.41,1.41,4.95),Vec3f(0.005,0.005,0.005));
}

TEST(VEC_PROJECTION_TEST){
    auto a = Vec3f{1,2,3};
    auto b = Vec3f{2,5,1};

    assert_equal(a.project(b), Vec3f(1,2.5f,0.5f));
}

TEST(VEC_ROTATION_TEST){
    auto a00 = Vec3f{7,0,2};
    auto b00 = Vec3f{1,1,0};
    auto radian00 = PI/4;
    auto r00 = a00.rotate(radian00, b00);
    assert_close(r00,Vec3f(4.97,2.03,4.91),Vec3f(0.005,0.005,0.005));

    auto a01 = Vec3f{2,-1,3};
    auto b01 = Vec3f{1,1,1};
    auto radian01 = PI/6;
    auto r01 = a01.rotate(radian01, b01);
    assert_close(r01,Vec3f(0.756,-0.3987,3.6427),Vec3f(0.0005,0.0005,0.0005));
}

TEST(VEC_COMPARISON_OPERATORS) {
    assert_equal(Vec3f(1, 2, 3) == Vec3f(1, 2, 3), Vec3u(1, 1, 1));
    assert_equal(Vec3f(1, 2, 3) != Vec3f(1, 2, 4), Vec3u(0, 0, 1));
    assert_equal(Vec3f(1, 5, 3) > Vec3f(0, 4, 3), Vec3u(1, 1, 0));
    assert_equal(Vec3f(1, 2, 3) <= Vec3f(1, 3, 3), Vec3u(1, 1, 1));
}

TEST(VEC_NEGATION_AND_ASSIGNMENT) {
    Vec3f v(1, -2, 3);
    assert_equal(-v, Vec3f(-1, 2, -3));
    v *= 2.0f;
    assert_equal(v, Vec3f(2, -4, 6));
    v /= 2.0f;
    assert_equal(v, Vec3f(1, -2, 3));
}

int main() { return TestRunner::instance().run("Vector Test"); }
