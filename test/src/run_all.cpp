#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * \file
 * \Run all tests in framework.
 */

/** File path separator */
static const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

/**
 * \brief Get all files in a directory.
 *
 * \param d Directory path.
 * \return File names.
 */
std::vector<std::string> get_directory_files(const std::string& d) {
    DIR*                     dir;
    struct dirent*           ent;
    std::vector<std::string> rv;
    if ((dir = opendir(d.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            // TODO: Check if directory
            rv.emplace_back(ent->d_name);
        }
        closedir(dir);
    } else {
        std::stringstream ss;
        ss << "Failed to open directory '" << d << "'";
        throw std::runtime_error(ss.str());
    }

    return rv;
}

/**
 * \brief Split file path into directory and file name + extension.
 * \param file_path File path.
 * \return {Directory, File name}
 */
std::pair<std::string, std::string> split_file_path(
    const std::string& file_path) {
    size_t      idx = file_path.find_last_of(sep);
    std::string file;
    std::string dir;
    if (idx == std::string::npos) {
        file = file_path;
        dir  = "";
    } else {
        file = file_path.substr(idx + 1);
        dir  = file_path.substr(0, idx);
    }

    return {file, dir};
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
        std::cout << "Usage: " << argv[0] << std::endl;
        return EXIT_SUCCESS;
    }

    /** Path to this executable */
    std::string path(argv[0]);

    // Get directory and file name
    auto        spl  = split_file_path(path);
    std::string dir  = spl.first;
    std::string name = spl.second;

    // Get all file names in directory
    std::vector<std::string> files = get_directory_files(dir);

    // Run all tests in directory
    for (const std::string& f : files) {
        // Sort out directories and this files
        // TODO: No need to check for . and .. if directories are sorted out
        if (f != "." && f != ".." && f != name) {
            int rv = std::system((dir + sep + f).c_str());
            if (rv == -1) {
                std::cout << "Failed to execute binary " << f << std::endl;
                return EXIT_FAILURE;
            } else if (rv != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}
