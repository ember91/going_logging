#include "goinglogging/goinglogging.h"
#include "test/test.h"

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    int m[2][2] = {{0, 1}, {2, 3}};

    D_MAT(m, 0, 0);
    D_MAT(m, 0, 1);
    D_MAT(m, 0, 2);
    D_MAT(m, 0, 0);
    D_MAT(m, 1, 0);
    D_MAT(m, 2, 0);
    D_MAT(m, 2, 2);

    return t.compare_output(false);
}

