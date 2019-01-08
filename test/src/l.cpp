#include <string>

#include "goinglogging/goinglogging.h"
#include "test/test.h"

/** Test outputting a class by overriding the << operator */
class test_op {
 public:
    friend std::ostream& operator<<(std::ostream& out, const test_op& t);
};

/** Output class information
 *
 * \param out Output stream
 * \param t   Object to output
 * \return    Stream
 *
 */
std::ostream& operator<<(std::ostream& out, const test_op& t) {
    (void)(t);
    out << "test_op";
    return out;
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    test t;
    t.setup(__FILE__);

    int         a = 1;
    const char* b = "hej";
    uint32_t    c = 7;
    int*        d = nullptr;
    bool        e = true;
    std::string f("asdsa");
    int*        g = &a;
    test_op     h;
    int&        i = a;
    l(a);
    l(a, b);
    l(a, b, c);
    l(a, b, c, d);
    l(a, b, c, d, e);
    l(a, b, c, d, e, f);
    l(a, b, c, d, e, f, g);
    l(a, b, c, d, e, f, g, h);
    l(a, b, c, d, e, f, g, h, i);
    l(a, b, c, d, e, f, g, h, i, *g);
    l(a, b, c, d, e, f, g, h, i, *g, static_cast<uint32_t>(a) + c);
    l(a, b, c, d, e, f, g, h, i, *g, static_cast<uint32_t>(a) + c, 0);
    l(a, b, c, d, e, f, g, h, i, *g, static_cast<uint32_t>(a) + c, 0, "j");

    return t.compare_output(true);
}

