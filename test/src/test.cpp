#include "test/test.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

static const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif  // _WIN32

std::string build_input_string(const std::string& f) {
    size_t idxSep = f.find_last_of(sep);
    if (idxSep == std::string::npos) {
        std::stringstream ss;
        ss << "Failed to find '" << sep << "' in path '" << f << "'";
        throw std::runtime_error(ss.str());
    }
    std::string s1 = f.substr(idxSep + 1);
    size_t idxDot = s1.find_last_of('.');
    std::string s2 = s1.substr(0, idxDot);
    return std::string("tmp_") + s2 + std::string(".txt");
}

std::string build_ground_truth_string(const std::string& f) {
    size_t idxSep = f.find_last_of(sep);
    if (idxSep == std::string::npos) {
        std::stringstream ss;
        ss << "Failed to find '" << sep << "' in path '" << f << "'";
        throw std::runtime_error(ss.str());
    }
    std::string s1 = f.substr(idxSep + 1);
    size_t idxDot = s1.find_last_of('.');
    std::string s2 = s1.substr(0, idxDot);
    return std::string("..") + sep + "ground_truth" + sep + s2 + std::string(".txt");
}

test::test() :
    m_file_name(""),
    m_stdout(nullptr) {
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

        // Both files ended at the same line
        if (!fGt && !fOut) {
            std::cout << "Files are identical" << std::endl;
            return EXIT_SUCCESS;
        }

        // Output file ended before ground truth
        if (fGt && !fOut) {
            std::cout << "Ground truth has more lines than output at line " <<
                         lineIdx << '\n' <<
                         "'" << lGt <<  "'" << std::endl;
            return EXIT_FAILURE;
        }

        // Ground truth ended before output file
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

    return EXIT_SUCCESS;
}
