#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif // _WIN32

/**
 * \file
 * Run all tests in framework.
 */

/** File path separator */
static const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

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

    return {dir, file};
}

/**
 * \brief Get all files in a directory.
 *
 * \param d Directory path.
 * \return File names.
 */
std::vector<std::string> get_directory_files(const std::string& d) {
    std::vector<std::string> rv;

#ifdef _WIN32
    std::stringstream ss;
    ss << d << sep << "*";

    WIN32_FIND_DATA fileData;
    HANDLE          handle = FindFirstFile(ss.str().c_str(), &fileData);
    if (handle != INVALID_HANDLE_VALUE) {
        // Iterate through all files in directory
        do {
            // Add to list if not folder
            if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=
                FILE_ATTRIBUTE_DIRECTORY) {
                rv.emplace_back(fileData.cFileName);
            }
        } while (FindNextFile(handle, &fileData));
    }

    if (handle != INVALID_HANDLE_VALUE) {
        FindClose(handle);
    }
#else
    DIR*           dir;
    struct dirent* ent;
    if ((dir = opendir(d.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string       file_name(ent->d_name);
            std::stringstream path;
            path << d << sep << file_name;
            struct stat statbuf; // Must use struct before variable name
            if (stat(path.str().c_str(), &statbuf) != 0) {
                std::stringstream ss;
                ss << "Failed to open directory '" << d
                   << "': " << strerror(errno);
                throw std::runtime_error(ss.str());
            }
            if (S_ISDIR(statbuf.st_mode))
                continue;
            rv.emplace_back(file_name);
        }
        closedir(dir);
    } else {
        std::stringstream ss;
        ss << "Failed to open directory '" << d << "'";
        throw std::runtime_error(ss.str());
    }
#endif // _WIN32

    return rv;
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
        // Sort out this file
        if (f != name) {
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
