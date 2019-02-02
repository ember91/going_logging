#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    int         a[2][2] = {{0, 1}, {2, 3}};
    const char* b[2][2] = {{"a", "b"}, {"c", "d"}};

    l_mat(a, 0, 0);
    l_mat(a, 0, 1);
    l_mat(a, 0, 2);
    l_mat(a, 0, 0);
    l_mat(a, 1, 0);
    l_mat(a, 2, 0);
    l_mat(a, 2, 2);
    l_mat(b, 2, 2);

    return t.compare_output(false);
}
