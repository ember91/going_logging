#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>

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

    int         a[3] = {0, 1, 2};
    const char* b[3] = {"a", "b", "c"};

    l_arr(a, 0);
    l_arr(a, 1);
    l_arr(a, 2);
    l_arr(a, 3);
    l_arr(b, 3);

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}
