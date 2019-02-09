#include "goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <string>

/**
 * \file
 * Test colored output functionality.
 */

using namespace gl::test;

/**
 * \brief Log variables.
 *
 * \param i Integer
 * \param a Array.
 * \param m Matrix.
 */
void log(int i, int a[1], int m[1][1]) {
    l(i);
    l(i, i);
    l_arr(a, 1);
    l_mat(m, 1, 1);
}

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

    // Set
    int i       = 1;
    int a[1]    = {1};
    int m[1][1] = {{1}};

    // Log
    gl::set_color_enabled(false);
    log(i, a, m);
    gl::set_color_enabled(true);
    log(i, a, m);
    gl::set_color_enabled(false);
    log(i, a, m);

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}