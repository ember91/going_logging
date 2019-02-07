#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <cstdint>
#include <cstdlib>

/**
 * \file
 * Test output of types in the C standard library.
 */

/**
 * \brief Log boolean.
 */
void log_bool() {
    bool bf = false;
    bool bt = true;

    l(bf);
    l(bt);
}

/**
 * \brief Log unsigned integers.
 */
void log_unsigned() {
    unsigned short     us  = 16;
    unsigned int       ui  = 32;
    unsigned long      ul  = 64;
    unsigned long long ull = 128;
    uint8_t            u8  = '8';
    uint16_t           u16 = 16;
    uint32_t           u32 = 32;
    uint64_t           u64 = 64;
    std::size_t        sz  = 1000;

    l(us);
    l(ui);
    l(ul);
    l(ull);
    l(u8);
    l(u16);
    l(u32);
    l(u64);
    l(sz);
}

/**
 * \brief Log signed integers.
 */
void log_signed() {
    signed short     ss  = 16;
    signed int       si  = 32;
    signed long      sl  = 64;
    signed long long sll = 128;
    int8_t           i8  = '8';
    int16_t          i16 = 16;
    int32_t          i32 = 32;
    int64_t          i64 = 64;
    std::ptrdiff_t   pd  = 5;

    l(ss);
    l(si);
    l(sl);
    l(sll);
    l(i8);
    l(i16);
    l(i32);
    l(i64);
    l(pd);
}

/**
 * \brief Log floating point types.
 */
void log_floating_point() {
    float       f32  = 32.0f;
    double      f64  = 64.0;
    long double f128 = 128.0L;

    l(f32);
    l(f64);
    l(f128);
}

/**
 * \brief Log string characters and types.
 */
void log_string() {
    unsigned char uc  = 'u';
    signed char   sc  = 's';
    char          c   = 'c';
    char16_t      c16 = '4';
    char32_t      c32 = '5';
    wchar_t       wc  = 'w';
    const char*   s1  = "s1";
    char*         s2  = const_cast<char*>(s1);

    l(uc);
    l(sc);
    l(c);
    l(c16);
    l(c32);
    l(wc);
    l(s1);
    l(s2);
}

/**
 * \brief Log time types.
 */
void log_time() {
    std::time_t  t1 = time(nullptr);
    std::clock_t cl = clock();
    std::tm      t2 = *std::localtime(&t1);

    l(t1);
    l(cl);
    l(t2);
}

/**
 * \brief Log math types.
 */
void log_math() {
    std::div_t   di   = {0, 1};
    std::ldiv_t  ldi  = {2, 3};
    std::lldiv_t lldi = {4, 5};

    l(di);
    l(ldi);
    l(lldi);
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

    log_bool();
    log_unsigned();
    log_signed();
    log_floating_point();
    log_string();
    log_time();
    log_math();

    // Don't test <stdatomic.h>, <stdnoreturn.h>, and <threads.h>, since they
    // are only in C but not in C++.
    // Don't test <ccomplex>, <complex.h>, <ctgmath>, and <tgmath.h>, since they
    // are empty, or deprecated.
    // Don't test <ciso646>, <iso646.h>, <cstdalign>, <stdalign.h>, <cstdbool>,
    // <stdbool.h>, sinze they are meaningless in C++.

    // Compare output
    return t.compare_output(Test::ComparisonMode::REGEX);
}
