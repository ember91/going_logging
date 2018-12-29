#include <cstdlib>
#include <dirent.h>
#include <stdexcept>
#include <string>
#include <vector>

#include <iostream>

std::vector<std::string> get_file_names(const std::string& d) {
    DIR* dir;
    struct dirent* ent;
    std::vector<std::string> rv;
    if ((dir = opendir(d.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            rv.emplace_back(ent->d_name);
        }
        closedir(dir);
    } else {
        throw std::runtime_error("Failed to open directory");
    }

    return rv;
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    std::string path(*argv);
    const char sep =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

    size_t idx = path.find_last_of(sep);
    std::string file;
    std::string dir;
    if (idx == std::string::npos) {
        file = path;
        dir  = "";
    } else {
        file = path.substr(idx + 1);
        dir  = path.substr(0, idx);
    }

    std::vector<std::string> files = get_file_names(dir);

    for (const std::string& f : files) {
        if (f != "." && f != ".." && f != file) {
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
