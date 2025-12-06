#include "test_tool.hpp"
#include <smath/smath.hpp>

using namespace smath;

TEST(PRINT_TEST) {
    std::cout << Mat2f(0, 1, 2, 3).toString() << std::endl;
    std::cout << Mat3f(0, 1, 2, 3, 4, 5, 6, 7, 8).toString() << std::endl;
}
TEST(ELEMENT_WISE_TEST){
    auto mat00 = Mat<3,2, float>::full(4.0f);
    auto mat01 = Mat<3,2, float>::full(4.0f);
    auto mat02 = Mat<3,2, int>::full(4);
    auto mat03 = Mat<3,2, int>::full(3);
    auto r00 = Mat<3,2, float>::full(8.0f);
    auto r01 = Mat<3,2, float>::full(0.0f);
    auto r02 = Mat<3,2, float>::full(16.0f);
    auto r03 = Mat<3,2, float>::full(1.0f);
    auto r04 = Mat<3,2, int>::full(1);
    assert_equal(mat00+mat01, r00);
    assert_equal(mat00-mat01, r01);
    assert_equal(mat00*mat01, r02);
    assert_equal(mat00/mat01, r03);
    assert_equal(mat02%mat03, r04);
}
TEST(TRANSPOSE_TEST) {
    assert_equal(Mat2f(0, 1, 2, 3).transpose(), Mat2f(0, 2, 1, 3));
}
TEST(DETERMINANT_TEST) {
    assert_equal(Mat2f(0, 0, 0, 0).determinant(), 0.0f);
    assert_equal(Mat2f(1, 0, 0, 1).determinant(), 1.0f);
    assert_equal(Mat2f(1, 2, 3, 4).determinant(), -2.0f);
    assert_equal(Mat3f(6.0f,4.0f,2.0f,
                       1.0f,-2.0f,8.0f,
                       1.0f,5.0f,7.0f).determinant(), -306.0f);
}
TEST(SUBMATRIX_TEST) {
    assert_equal(Mat2f(1, 2, 3, 4).subMatrixAt(0, 0), Mat1f(4));
    assert_equal(Mat2f(1, 2, 3, 4).subMatrixAt(1, 1), Mat1f(1));
    assert_equal(Mat3f(0, 1, 2, 3, 4, 5, 6, 7, 8).subMatrixAt(2, 0),
                 Mat2f(1, 2, 4, 5));
    assert_equal(Mat4f(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                     .subMatrixAt(0, 0),
                 Mat3f(5, 6, 7, 9, 10, 11, 13, 14, 15));
    assert_equal(Mat4f(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                     .subMatrixAt(1, 0),
                 Mat3f(1, 2, 3, 9, 10, 11, 13, 14, 15));
    assert_equal(Mat4f(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                     .subMatrixAt(3, 0),
                 Mat3f(1, 2, 3, 5, 6, 7, 9, 10, 11));
    assert_equal(Mat4f(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                     .subMatrixAt(2, 1),
                 Mat3f(0, 2, 3, 4, 6, 7, 12, 14, 15));
    assert_equal(Mat4f(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                     .subMatrixAt(3, 3),
                 Mat3f(0, 1, 2, 4, 5, 6, 8, 9, 10));
}

TEST(MULTIPLICATION_TEST) {
    auto mat00 = Mat2f(0, 1, 2, 3);
    auto mat01 = Mat<2, 1, float>(1, 2);
    auto r0 = Mat<2, 1, float>(4, 7);
    assert_equal(mat00.cross(mat01), r0);
    auto mat10 = Mat2f(0, 1, 2, 3);
    auto mat11 = Mat2f(4, 1, 2, -1);
    auto r1 = Mat2f(2, 7, -2, -1);
    assert_equal(mat10.cross(mat11), r1);
    auto mat20 = Mat2f(1, 2, 3, 4);
    auto mat21 = Mat2f::identity();
    auto r2 = mat20;
    assert_equal(mat20.cross(mat21), r2);
    auto mat30 = Mat3f(3, 2, -6, 7, 8, 1, 1, 0, -2);
    auto mat31 = Mat3f(1, -1, 7, 0, 0, -2, 5, 1, 2);
    auto r3 = Mat3f(3, -6, -21, -2, 0, 4, 24, 18, -33);
    assert_equal(mat30.cross(mat31), r3);
}
TEST(MAT_TO_VEC_TEST) {
    auto mat00 = Mat3f(1, 2, 3, 4, 5, 6, 7, 8, 9);
    auto r00 = mat00.toVectors()[0] == Vec3f(1, 2, 3);
    assert_equal(mat00.toVectors()[0], Vec3f(1, 2, 3));
    assert_equal(mat00.toVectors()[1], Vec3f(4, 5, 6));
    assert_equal(mat00.toVectors()[2], Vec3f(7, 8, 9));

    auto mat01 = Mat<3, 2, float>(1, 2, 3, 4, 5, 6);
    assert_equal(mat01.toVectors()[0], Vec3f(1, 2, 3));
    assert_equal(mat01.toVectors()[1], Vec3f(4, 5, 6));
}

TEST(INVERSION_TEST) {
    auto mat00 = Mat2f(1.0f, 2.0f, 3.0f, 4.0f);
    auto r00 = Mat2f(-2.0f, 1.0f, 1.5f, -0.5f);
    assert_equal(mat00.inverse(), r00);
};

int main() { return TestRunner::instance().run("Mat Test"); }
