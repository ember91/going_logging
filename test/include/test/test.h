#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_

#include <fstream>
#include <streambuf>
#include <string>

namespace gl {
namespace test {

/**
 * \brief Setup testing environment for comparing logging output to ground
 * truth.
 */
class Test {
  public:
    /** How to compare log output and ground truth. */
    enum class ComparisonMode {
        EXACT, /**< Match exactly between ground truth and log output. */
        REGEX  /**< Match regex in ground truth file to log output. */
    };

    Test();
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;
    virtual ~Test();
    void setup(const std::string& f);
    int  compare_output(ComparisonMode cmp);

  private:
    /** Name of test file. */
    std::string m_file_name;
    /** std::cout. */
    std::streambuf* m_stdout;
    /** Log output file. */
    std::ofstream m_output;
};

} // namespace test
} // namespace gl

#endif // INCLUDE_TEST_H_
