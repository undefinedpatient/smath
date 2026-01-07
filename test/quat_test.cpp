#include "test_tool.hpp"
#include "smath.hpp"

using namespace smath;
TEST(PRINT_TEST) {
    std::cout<< Quat<float>{1,2,3,4}.to_string() << std::endl;
}
TEST(QUAT_BASIC_CONSTRUCTION) {
    Quat<float> q0; // default
    assert_equal(q0, Quat<float>(0, 0, 0, 0));

    Quat<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quat<float> q11{1.0f,2.0f,3.0f,4.0f};
    assert_equal(q1, q11);

    Quat<float> q2{5.0f, 6.0f, 7.0f, 8.0f}; // initializer list
    assert_equal(q2, Quat<float>(5.0f, 6.0f, 7.0f, 8.0f));

    Quat<float> q3 = q1; // copy
    assert_equal(q3, q1);
}


TEST(QUAT_NORM_AND_NORMALIZE) {
    Quat<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    assert_close(q.length2(), 30.0f, 1e-6f);
    assert_close(q.length(), std::sqrt(30.0f), 1e-6f);

    Quat<float> normalized = q.normalize();
    assert_close(normalized.length(), 1.0f, 1e-5f);
}

TEST(QUAT_CONJUGATE_AND_INVERSE) {
    Quat<float> unit(0.5f, 0.5f, 0.5f, 0.5f); // norm = 1
    assert_equal(unit.conjugate(), Quat<float>(0.5f, -0.5f, -0.5f, -0.5f));
    assert_equal(unit.inverse(), unit.conjugate());

    Quat<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    Quat<float> inv = q.inverse();
    assert_equal(q.mul(inv), Quat<float>(1.0f, 0.0f, 0.0f, 0.0f)); // within eps
}

TEST(QUAT_MULTIPLY) {
    // Standard quaternion multiplication test (i * j = k)
    Quat<float> i(0.0f, 1.0f, 0.0f, 0.0f);
    Quat<float> j(0.0f, 0.0f, 1.0f, 0.0f);
    Quat<float> k(0.0f, 0.0f, 0.0f, 1.0f);

    assert_equal(i.mul(j), k);
    assert_equal(j.mul(i), -k);
    assert_equal(k.mul(i), j);

    // Operator * (currently element-wise — see note below)
    // We'll test the correct mul() instead
    Quat<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quat<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    Quat<float> expected(
        1*5 - 2*6 - 3*7 - 4*8,
        1*6 + 2*5 + 3*8 - 4*7,
        1*7 + 3*5 + 4*6 - 2*8,
        1*8 + 4*5 + 2*7 - 3*6
    );
    assert_equal(q1.mul(q2), expected);
}

TEST(QUAT_TO_MATRIX) {
    // 180° rotation around X axis
    Quat<float> q(0.0f, 1.0f, 0.0f, 0.0f); // cos(90°)=0, sin(90°)=1
    Mat<3,3,float> expected{
        1,  0,  0,
        0, -1,  0,
        0,  0, -1
    };
    Mat<3,3,float> m3 = q.to_mat3();
    Mat<3,3,float> t3 = Mat<3,3,float>(1e-2f);
    assert_close(m3, expected, t3);

    Mat<4,4,float> m4 = q.to_mat4();
    Mat<4,4,float> expected4{
        1,  0,  0, 0,
        0, -1,  0, 0,
        0,  0, -1, 0,
        0,  0,  0, 1
    };
    // Similar matrix comparison
}

TEST(QUAT_FROM_MATRIX) {
    // Identity rotation
    Mat<3,3,float> identity = Mat<3,3,float>::identity();
    Quat<float> q = Quat<float>().from_mat3(identity).normalize_or_one();
    assert_equal(q, Quat<float>(1.0f, 0.0f, 0.0f, 0.0f)); // or normalized equivalent
}

TEST(QUAT_SLERP) {
    Quat<float> q0(1.0f, 0.0f, 0.0f, 0.0f); // identity
    Quat<float> q1(0.0f, 1.0f, 0.0f, 0.0f); // 180° around X

    Quat<float> mid = slerp(q0, q1, 0.5f);
    // Should be ~90° rotation around X
    assert_close(mid.length(), 1.0f, 1e-5f);
    assert_close(mid.scalar(), static_cast<float>(std::cos(PI/4)), 1e-5f);
    assert_close(mid[1], static_cast<float>(std::sin(PI/4)), 1e-5f);
}

TEST(BOOLEAN){
    Quat<float> q0{1.0f, 0.0f, 0.0f, 0.0f}; // identity
    Quat<float> q1{0.0f, 0.0f, 0.0f, 0.0f}; // 180° around X
    assert_equal(q0.all(), false);
    assert_equal(q0.any(), true);
    assert_equal(q0.none(), false);
    assert_equal(q1.all(), false);
    assert_equal(q1.any(), false);
    assert_equal(q1.none(), true);
}

int main() { return TestRunner::instance().run("Quaternion Test"); }
