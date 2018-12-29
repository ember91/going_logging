#include "test/test.h"

#include <iostream>
#include <regex>

static const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif  // _WIN32

std::string build_input_string(const std::string& f) {
    std::string s1 = f.substr(f.find_last_of(sep) + 1);
    std::string s2 = s1.substr(0, s1.find_last_of('.'));
    return std::string("tmp_") + s2 + std::string(".txt");
}

std::string build_ground_truth_string(const std::string& f) {
    std::string s1 = f.substr(f.find_last_of(sep) + 1);
    std::string s2 = s1.substr(0, s1.find_last_of('.'));
    return std::string("..") + sep + "ground_truth" + sep + s2 + std::string(".txt");
}

test::test() :
    m_file_name(""),
    m_stdout(nullptr),
    m_output() {
}

test::~test() {
    std::remove(build_input_string(m_file_name).c_str());
}

void test::setup(const std::string& f) {
    m_file_name = f;
    m_output.open(build_input_string(m_file_name));
    if (m_stdout == nullptr) {
        m_stdout = std::cout.rdbuf();
    }
    std::cout.rdbuf(m_output.rdbuf());
}

int test::compare_output(bool regex) {
    m_output.flush();
    m_output.close();

    std::string out = build_input_string(m_file_name);
    std::string gt  = build_ground_truth_string(m_file_name);

    if (stdout != nullptr) {
        std::cout.rdbuf(m_stdout);
    }

    std::ifstream fOut(out);
    std::ifstream fGt(gt);

    if (!fOut.is_open()) {
        std::cout << "Failed to open '" << out <<  "'" << std::endl;
        return EXIT_FAILURE;
    }

    if (!fGt.is_open()) {
        std::cout << "Failed to open '" << gt <<  "'" << std::endl;
        return EXIT_FAILURE;
    }

    std::string lOut;
    std::string lGt;

    uint32_t lineIdx = 1;

    std::cout << "Comparing '" << out << "' and '" << gt << "': ";

    while (true) {
        std::getline(fOut, lOut);
        std::getline(fGt, lGt);

        if (!fGt && !fOut) {
            std::cout << "Files are identical" << std::endl;
            return EXIT_SUCCESS;
        }

        if (fGt && !fOut) {
            std::cout << "Ground truth has more lines than output at line " <<
                         lineIdx << '\n' <<
                         "'" << lGt <<  "'" << std::endl;
            return EXIT_FAILURE;
        }

        if (!fGt && fOut) {
            std::cout << "Output has more lines than ground truth at line " <<
                         lineIdx << '\n' <<
                         "'" << lOut <<  "'" << std::endl;
            return EXIT_FAILURE;
        }

        if (regex) {
            std::regex rx(lGt);
            if (!std::regex_match(lOut, rx)) {
                std::cout << "Regular expression mismatch at line " <<
                             lineIdx << '\n' <<
                             "'" << lOut << "'\n" <<
                             "'" << lGt << "'" << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            if (lOut != lGt) {
                std::cout << "Mismatch at line " << lineIdx << '\n' <<
                             "'" << lOut << "'\n" <<
                             "'" << lGt << "'" << std::endl;
                return EXIT_FAILURE;
            }
        }

        lineIdx++;
    }
}

