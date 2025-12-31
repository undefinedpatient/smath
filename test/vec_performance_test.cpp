#include "test_tool.hpp"
#include "smath.hpp"
using namespace smath;
TEST(VECTOR_ELEMENT_WISE_ADDITION_PERFORMANCE){
    auto base = Vec<16,unsigned int>();
    auto target = Vec<16,unsigned int>(42000000);
    for(unsigned int i = 0; i<42000000;i++){
        base += Vec<16,unsigned int>(1);
    }

    assert_equal(base, target);
};
int main(){return TestRunner::instance().run("Vector Performance");}
