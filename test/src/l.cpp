#include "goinglogging/goinglogging.h"
#include "test/test.h"

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

    int i0  = 0;
    int i1  = 1;
    int i2  = 2;
    int i3  = 3;
    int i4  = 4;
    int i5  = 5;
    int i6  = 6;
    int i7  = 7;
    int i8  = 8;
    int i9  = 9;
    int i10 = 10;
    int i11 = 11;
    int i12 = 12;
    int i13 = 13;
    int i14 = 14;
    int i15 = 15;

    l(i0);
    l(i0, i1);
    l(i0, i1, i2);
    l(i0, i1, i2, i3);
    l(i0, i1, i2, i3, i4);
    l(i0, i1, i2, i3, i4, i5);
    l(i0, i1, i2, i3, i4, i5, i6);
    l(i0, i1, i2, i3, i4, i5, i6, i7);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14);
    l(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15);

    const char* s1 = "s1";
    const char* s2 = "s2";

    l(s1);
    l(s1, s2);

    // Compare output
    return t.compare_output(Test::ComparisonMode::REGEX);
}
