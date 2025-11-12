#include <smath/smath.hpp>
#include "message.hpp"

void test_vec() {
    using namespace smath;
    print_message('-',"Vec Test");
    Vec3f v1 = Vec3f(2, 4, 4);
    assert_equal(v1.length(), 6.0f);
    assert_equal(v1/3, Vec3f(1,2,2));
    assert_equal(v1/2, Vec3f(1,2,2));
}
