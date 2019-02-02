#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <iostream>
#include <ostream>
#include <string>

/** Test outputting a class by overriding the << operator */
class TestOp {
  public:
    friend std::ostream& operator<<(std::ostream& out, const TestOp& t);
};

/** Output class information
 *
 * \param out Output stream
 * \param t   Object to output
 * \return    Stream
 *
 */
std::ostream& operator<<(std::ostream& out, const TestOp& t) {
    (void)(t);
    out << "TestOp";
    return out;
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    int         a = 1;
    const char* b = "hej";
    uint32_t    c = 7;
    int*        d = nullptr;
    bool        e = true;
    std::string f("asdsa");
    int*        g = &a;
    TestOp      h;
    int&        i = a;
    char        j = 'j';

    l(a);
    l(a, b);
    l(a, b, c);
    l(a, b, c, d);
    l(a, b, c, d, e);
    l(a, b, c, d, e, f);
    l(a, b, c, d, e, f, g);
    l(a, b, c, d, e, f, g, h);
    l(a, b, c, d, e, f, g, h, i);
    l(a, b, c, d, e, f, g, h, i, j);
    l(a, b, c, d, e, f, g, h, i, j, *g);
    l(a, b, c, d, e, f, g, h, i, j, *g, a);
    l(a, b, c, d, e, f, g, h, i, j, *g, a, b);

    return t.compare_output(true);
}
