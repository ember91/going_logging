#include "goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>

/**
 * \file
 * Test enabling and disabling output.
 */

/**
 * \brief Log variables.
 *
 * \param i Integer.
 * \param a Array.
 * \param m Matrix.
 */
void log(int i, int a[2], int m[2][2]) {
    l(i);
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

    // Log
    int i       = 0;
    int a[2]    = {0};
    int m[2][2] = {{0}};
    gl::set_output_enabled(true);
    log(i, a, m);
    i++;
    a[0]++;
    m[0][0]++;
    gl::set_output_enabled(false);
    log(i, a, m);
    i++;
    a[0]++;
    m[0][0]++;
    gl::set_output_enabled(true);
    log(i, a, m);

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}
