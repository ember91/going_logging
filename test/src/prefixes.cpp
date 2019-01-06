#include "goinglogging/goinglogging.h"
#include "test/test.h"

#define PERM(n, p) \
    for (gl::prefix (n) = gl::prefix::NONE; (n) <= (p); \
        (n) = static_cast<gl::prefix>(static_cast<uint32_t>((n)) + \
                                      static_cast<uint32_t>((p))))
int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    int a[1]    = {0};
    int m[1][1] = {{0}};

    PERM(fi, gl::prefix::FILE) {
        PERM(l, gl::prefix::LINE) {
            PERM(fu, gl::prefix::FUNCTION) {
                PERM(ti, gl::prefix::TIME) {
                    PERM(th, gl::prefix::THREAD) {
                        gl::prefix p = (fi | l | fu | ti | th);
                        gl::set_prefixes(p);
                        if (p != gl::get_prefixes()) {
                            std::cout << "Failed to set prefixes" << std::endl;
                            return EXIT_FAILURE;
                        }
                        l(0);
                        l_arr(a, 1);
                        l_mat(m, 1, 1);
                    }
                }
            }
        }
    }

    return t.compare_output(true);
}

