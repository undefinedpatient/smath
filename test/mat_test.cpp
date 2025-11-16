#include <smath/smath.hpp>
#include "smath/mat.hpp"
#include "test_tool.hpp"

using namespace smath;

TEST(PRINT_TEST){
    std::cout<<Mat2f(0,1,2,3).toString()<<std::endl;
    std::cout<<Mat3f(0,1,2,3,4,5,6,7,8).toString()<<std::endl;
}
TEST(TRANSPOSE_TEST){
    assert_equal(Mat2f(0,1,2,3).transpose(), Mat2f(0,2,1,3));
}
TEST(DETERMINANT_TEST){
    assert_equal(Mat2f(0,0,0,0).determinant(),0.0f);
    assert_equal(Mat2f(1,0,0,1).determinant(),1.0f); 
}
TEST(SUBMATRIX_TEST){
    assert_equal(
        Mat3f(
            0,1,2,
            3,4,5,
            6,7,8
        ).subMatrixAt(2, 0),
        Mat2f(
            1,2,
            4,5
        )
    );
    assert_equal(
        Mat4f(
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15
        ).subMatrixAt(0, 0),
        Mat3f(
            5,6,7,
            9,10,11,
            13,14,15
        )
    );
    assert_equal(
        Mat4f(
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15
        ).subMatrixAt(1, 0),
        Mat3f(
            1,2,3,
            9,10,11,
            13,14,15
        )
    );
    assert_equal(
        Mat4f(
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15
        ).subMatrixAt(3, 0),
        Mat3f(
            1,2,3,
            5,6,7,
            9,10,11
        )
    );
    assert_equal(
        Mat4f(
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15
        ).subMatrixAt(2, 1),
        Mat3f(
            0,2,3,
            4,6,7,
            12,14,15
        )
    );
    assert_equal(
        Mat4f(
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15
        ).subMatrixAt(3, 3),
        Mat3f(
            0,1,2,
            4,5,6,
            8,9,10
        )
    );

}

TEST(MULTIPLICATION_TEST){
    auto mat00 = Mat2f(0,1,2,3);
    auto mat01 = Mat<2,1,float>(1,2);
    auto r0 = Mat<2,1,float>(4,7);
    assert_equal(
        mat00*mat01,
        r0
    );
    auto mat10 = Mat2f(0,1,2,3);
    auto mat11 = Mat2f(4,1,2,-1);
    auto r1 = Mat2f(2,7,-2,-1);
    assert_equal(
        mat10*mat11,
        r1
    );
}
int main(){
    return TestRunner::instance().run("Mat Test");
}
