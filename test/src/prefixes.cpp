#include "goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>

/**
 * \file
 * Test prefix functionality.
 *
 */

using namespace gl::test;

/**
 * \brief Run code in a block with prefix disabled and enabled.
 * \param v Variable to put prefix in.
 * \param p Prefix type.
 */
#define BOTH(v, p)                                     \
    for (gl::prefix(v) = gl::prefix::NONE; (v) <= (p); \
         (v)           = static_cast<gl::prefix>(      \
             static_cast<uint32_t>((v)) + static_cast<uint32_t>((p))))

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

    Test t;
    t.setup(__FILE__);

    // Outputs
    int i       = 0;
    int a[1]    = {0};
    int m[1][1] = {{0}};

    // Test all combinations
    BOTH(fi, gl::prefix::FILE) {
        BOTH(l, gl::prefix::LINE) {
            BOTH(fu, gl::prefix::FUNCTION) {
                BOTH(ti, gl::prefix::TIME) {
                    BOTH(th, gl::prefix::THREAD) {
                        BOTH(tn, gl::prefix::TYPE_NAME) {
                            gl::prefix p = (fi | l | fu | ti | th | tn);
                            gl::set_prefixes(p);
                            if (p != gl::get_prefixes()) {
                                std::cout << "Failed to set prefixes"
                                          << std::endl;
                                return EXIT_FAILURE;
                            }
                            l(i);
                            l_arr(a, 1);
                            l_mat(m, 1, 1);
                        }
                    }
                }
            }
        }
    }

    // Compare output
    return t.compare_output(Test::ComparisonMode::REGEX);
}