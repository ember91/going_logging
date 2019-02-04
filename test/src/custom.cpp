#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

/** \brief Overrides the stream insertion operator */
class CustInsOp {
  public:
    /** \brief Constructor.
     *
     * \param i Test value.
     *
     */
    explicit CustInsOp(int i) : m_i(i) {
    }

    friend std::ostream& operator<<(std::ostream& os, const CustInsOp& c);

  private:
    int m_i; /**< Test value. */
};

/** Insert into stream.
 *
 * \param os Output stream.
 * \param c  Custom insertion operator object.
 * \return   Stream.
 *
 */
std::ostream& operator<<(std::ostream& os, const CustInsOp& c) {
    return os << "<CustInsOp: " << c.m_i << '>';
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    CustInsOp c(5);
    l(c);

    return t.compare_output(false);
}
