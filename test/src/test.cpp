#include "test/test.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

/** File separator character. */
static const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif // _WIN32

/**
 * \brief Get file name without extension from file path.
 * \param p File path.
 * \return File name withpout extension.
 */
std::string file_name(const std::string& p) {
    size_t idxSep = p.find_last_of(sep);
    if (idxSep == std::string::npos) {
        idxSep = 0;
    }

    std::string withExt = p.substr(idxSep + 1);
    size_t      idxDot  = withExt.find_last_of('.');
    std::string name    = withExt.substr(0, idxDot);

    return name;
}

/**
 * \brief Get file name of log file from file path.
 * \param p File path.
 * \return Log file name.
 */
std::string build_log_file_name(const std::string& p) {
    return std::string("tmp_") + file_name(p) + std::string(".txt");
}

/**
 * \brief Get file name of ground truth file from file path.
 * \param p File path.
 * \return Log file name.
 */
std::string build_ground_truth_file_name(const std::string& p) {
    return std::string("..") + sep + "ground_truth" + sep + file_name(p) +
           std::string(".txt");
}

/**
 * \brief Constructor.
 */
Test::Test() : m_file_name(""), m_stdout(nullptr) {
}

/**
 * \brief Destructor. Cleanup temporary log file.
 */
Test::~Test() {
    std::remove(build_log_file_name(m_file_name).c_str());
}

/**
 * \brief Setup test by redirecting std::cout to temporary file.
 *
 * \param f Name of test file.
 */
void Test::setup(const std::string& f) {
    m_file_name = f;
    m_output.open(build_log_file_name(m_file_name));
    if (m_stdout == nullptr) {
        m_stdout = std::cout.rdbuf();
    }
    std::cout.rdbuf(m_output.rdbuf());
}

/**
 * \brief Compare output between log file and ground truth.
 *
 * \param cmp How to compare log output and ground truth.
 * \return EXIT_SUCCESS if success.
 */
int Test::compare_output(ComparisonMode cmp) {
    // Ensure outout is flushed
    m_output.flush();
    m_output.close();

    // Get file names
    std::string out = build_log_file_name(m_file_name);
    std::string gt  = build_ground_truth_file_name(m_file_name);

    // Reset output to stdout
    if (stdout != nullptr) {
        std::cout.rdbuf(m_stdout);
    }

    // Create streams
    std::ifstream fOut(out);
    std::ifstream fGt(gt);

    // Open files
    if (!fOut.is_open()) {
        std::cout << "Failed to open '" << out << "'" << std::endl;
        return EXIT_FAILURE;
    }
    if (!fGt.is_open()) {
        std::cout << "Failed to open '" << gt << "'" << std::endl;
        return EXIT_FAILURE;
    }

    // Lines to compare
    std::string lOut;
    std::string lGt;

    /** Current line number in file */
    uint32_t lineNumber = 1;

    std::cout << "Comparing '" << out << "' and '" << gt << "': ";

    while (true) {
        // Read lines
        std::getline(fOut, lOut);
        std::getline(fGt, lGt);

        // Both files ended at the same line
        if (!fGt && !fOut) {
            std::cout << "Files are identical" << std::endl;
            return EXIT_SUCCESS;
        }

        // Output file ended before ground truth
        if (fGt && !fOut) {
            std::cout << "Ground truth has more lines than output at line "
                      << lineNumber << '\n'
                      << "Grt: '" << lGt << '\'' << std::endl;
            return EXIT_FAILURE;
        }

        // Ground truth ended before output file
        if (!fGt && fOut) {
            std::cout << "Output has more lines than ground truth at line "
                      << lineNumber << '\n'
                      << "Out: '" << lOut << '\'' << std::endl;
            return EXIT_FAILURE;
        }

        // Compare differently depending on if regex or not
        if (cmp == ComparisonMode::REGEX) {
            try {
                std::regex rx(lGt);
                if (!std::regex_match(lOut, rx)) {
                    std::cout << "Regular expression mismatch at line "
                              << lineNumber << '\n'
                              << "Out: '" << lOut << "'\n"
                              << "Grt: '" << lGt << '\'' << std::endl;
                    return EXIT_FAILURE;
                }
            } catch (const std::regex_error& exc) {
                std::cout << "Regular expression exception at line "
                          << lineNumber << '\n'
                          << "Grt: '" << lGt << "': " << exc.what()
                          << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            if (lOut != lGt) {
                std::cout << "Mismatch at line " << lineNumber << '\n'
                          << "Out: '" << lOut << "'\n"
                          << "Grt: '" << lGt << '\'' << std::endl;
                return EXIT_FAILURE;
            }
        }

        // Increment line number
        lineNumber++;
    }

    return EXIT_SUCCESS;
}
