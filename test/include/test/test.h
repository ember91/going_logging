#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_

#include <fstream>
#include <streambuf>
#include <string>

class test {
  public:
    test();
    test(const test&) = delete;
    test& operator=(const test&) = delete;
    virtual ~test();
    void setup(const std::string& f);
    int compare_output(bool regex);

  private:
    std::string     m_file_name;
    std::streambuf* m_stdout;
    std::ofstream   m_output;
};

#endif  // INCLUDE_TEST_H_
