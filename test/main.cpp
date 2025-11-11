#include <iostream>

#include "message.hpp"
#include "smath/smath.hpp"

void test_vec3();
int main() {
    print_message('=', "Smath Testing");
    test_vec3();
    print_message('=', "All Test Done");
    return 0;
}
