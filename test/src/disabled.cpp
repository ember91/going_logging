#define GOINGLOGGING_DISABLED
#include "goinglogging/goinglogging.h"
#include "test/test.h"

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    int i = 2;
    D(i);

    int a[2] = {0, 1};
    D_ARR(a, 2);

    int m[2][2] = {{0, 1}, {2, 3}};
    D_MAT(m, 2, 2);

    return t.compare_output(false);
}
