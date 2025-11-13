#include <smath/smath.hpp>
#include "test_tool.hpp"

TEST(Vector_Length_Test){
    
};
// TEST_TEST(name){
    
// };
TEST_TEST_TEST(name){
    
};
int main() {
    using namespace smath;
    print_message('-',"Vec Test");
    Vec3f v1 = Vec3f(2, 4, 4);

    assert_equal(v1.length(), 6.0f);
    return 0;
}
