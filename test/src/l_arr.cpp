#include <iostream>
#include <memory>
#include <ostream>

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

    l_arr(a,  0);
    l_arr(a,  1);
    l_arr(a,  2);
    l_arr(a,  3);
    l_arr(a2.get(), 1);

    return t.compare_output(false);
}

