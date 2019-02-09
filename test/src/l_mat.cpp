#include "goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>

/**
 * \file
 * Test output of l_mat().
 */

using namespace gl::test;

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

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}