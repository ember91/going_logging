#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <cstdint>
#include <cstdlib>

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    // Boolean
    bool b = true;

    // Unsigned
    unsigned char      uc  = 'u';
    unsigned short     us  = 16;
    unsigned int       ui  = 32;
    unsigned long      ul  = 64;
    unsigned long long ull = 128;
    uint8_t            u8  = '8';
    uint16_t           u16 = 16;
    uint32_t           u32 = 32;
    uint64_t           u64 = 64;

    // Signed
    signed char      sc  = 'u';
    signed short     ss  = 16;
    signed int       si  = 32;
    signed long      sl  = 64;
    signed long long sll = 128;
    int8_t           i8  = '8';
    int16_t          i16 = 16;
    int32_t          i32 = 32;
    int64_t          i64 = 64;

    // Other integer types
    size_t    sz = 1000;
    ptrdiff_t pd = 5;

    // Floating point
    float       f32  = 32.0f;
    double      f64  = 64.0;
    long double f128 = 128.0L;

    // String
    char        c8  = '1';
    char16_t    c16 = '4';
    char32_t    c32 = '5';
    wchar_t     wc  = 'w';
    const char* s1  = "s1";
    char*       s2  = const_cast<char*>(s1);

    // Time
    time_t    t1 = time(nullptr);
    clock_t   cl = clock();
    struct tm t2 = *std::localtime(&t1);

    // Math
    div_t   di   = {0, 1};
    ldiv_t  ldi  = {2, 3};
    lldiv_t lldi = {4, 5};

    // Atomic
    // No need to test, since they work the same as other types

    // Exclusions
    // Don't test <stdatomic.h>, <stdnoreturn.h>, and <threads.h>, since they
    // are only in C but not in C++
    // Don't test <ccomplex>, <complex.h>, <ctgmath>, and <tgmath.h>, since they
    // are empty, or deprecated.
    // Don't test <ciso646>, <iso646.h>, <cstdalign>, <stdalign.h>, <cstdbool>,
    // <stdbool.h>, sinze they are meaningless in C++

    // Log
    l(b);
    l(uc);
    l(us);
    l(ui);
    l(ul);
    l(ull);
    l(u8);
    l(u16);
    l(u32);
    l(u64);
    l(sc);
    l(ss);
    l(si);
    l(sl);
    l(sll);
    l(i8);
    l(i16);
    l(i32);
    l(i64);
    l(sz);
    l(pd);
    l(f32);
    l(f64);
    l(f128);
    l(c8);
    l(c16);
    l(c32);
    l(wc);
    l(s1);
    l(s2);
    l(t1);
    l(cl);
    l(t2);
    l(di);
    l(ldi);
    l(lldi);

    return t.compare_output(true);
}
