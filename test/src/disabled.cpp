#include "goinglogging/goinglogging.h"
#include "test/test.h"

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    gl::set_enabled(false);

    int i = 2;
    l(i);

    int a[2] = {0, 1};
    l_arr(a, 2);

    int m[2][2] = {{0, 1}, {2, 3}};
    l_mat(m, 2, 2);

    int j = 2;

    gl::set_enabled(true);

    l(j);

    return t.compare_output(false);
}

