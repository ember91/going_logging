#include "goinglogging.h"
#include "test/test.h"
#include <ostream>

/**
 * \file
 * Test custom types with overloaded stream insertion operator.
 */

/**
 * \brief Overrides the stream insertion operator.
 */
class CustInsOp {
  public:
    /**
     * \brief Constructor.
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

/**
 * \brief Insert into stream.
 *
 * \param os Output stream.
 * \param c  Custom insertion operator object.
 * \return   Output stream.
 *
 */
std::ostream& operator<<(std::ostream& os, const CustInsOp& c) {
    return os << "<CustInsOp: " << c.m_i << '>';
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

    // Log
    CustInsOp c(5);
    l(c);

    // Compare output
    return t.compare_output(Test::ComparisonMode::EXACT);
}
