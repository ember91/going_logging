#include <memory>

#include "goinglogging/goinglogging.h"
#include "test/test.h"
int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    int a[4] = {0, 1, 2};
    std::unique_ptr<int[]> a2(new int[1]);
    a2[0] = 0;

    D_ARR(a,  0);
    D_ARR(a,  1);
    D_ARR(a,  2);
    D_ARR(a,  3);
    D_ARR(a2.get(), 1);

    return t.compare_output(false);
}

