#include <iostream>
#include <ostream>

#define GL_NEWLINE " end\n"
#include "goinglogging.h"
#include "test/test.h"

/**
 * \brief Test entry point.
 *
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \return EXIT_SUCCESS if success.
 */
int main(int argc, const char** argv) {
    // Check number of arguments
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    // Disable prefixes for easier output comparison.
    gl::set_prefixes(gl::prefix::NONE);

    Test t;
    t.setup(__FILE__);

    int i = 1;
    l(i);

    int a[2] = {0, 1};
    l_arr(a, 2);

    int m[2][2] = {{0, 1}, {2, 3}};
    l_mat(m, 2, 2);

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}
