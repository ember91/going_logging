#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <string>

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    int i       = 1;
    int a[1]    = {1};
    int m[1][1] = {{1}};

    gl::set_color_enabled(false);
    l(i);
    l(i, i);
    l_arr(a, 1);
    l_mat(m, 1, 1);

    gl::set_color_enabled(true);
    l(i);
    l(i, i);
    l_arr(a, 1);
    l_mat(m, 1, 1);

    gl::set_color_enabled(false);

    return t.compare_output(false);
}
