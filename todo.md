# Cleanup

* Improve documentation
* Compile with all warnings enabled
* Run static analysis
* Run valgrind
* Add noexcept, const, constepxr, explicit to functions
* Add const to parameters and members where applicable
* No tabs
* Test on Windows
* Clean up CMakeLists.txt
* Forward declare all functions
* Check limit of 80 characters
* Ensure classes are PascalCase, variables camelCase and functions snake_case
* Inline functions in classes?
* Ensure one character c-strings use single quotes
* Ensure almost no usage of std::cout
* Use ::gl::internal instead of gl::internal
* Fix order of functions and classes
* Pass std::string by reference only when necessary
* Ensure documentation such as \param ends with dot
* Don't convert between const char* and std::string too much
* Don't use C-style comments
* Ensure parameters are in a good order
* Control regexes in test ground truth
* Include what's used but nothing more
* Neither getterrs nor setters should use \brief
* Ensure template parameters are documented with \tparam
* Make functions that can be static static
* Use C++11 functionality
* Ensure relevant features are listed in both doxygen and README

# Features

# Data type prefix

* Readd functionality for this, but this time smarter

## VS code

* Build with CMake from IDE
* Launch binary from IDE

## Stringification

* Add C++ types

## Documentation

* Add problem description (printf("i = %s\n"); vs l(i);)
* Better looking doxygen theme

## Build system

* Run doxygen in cmake

## Version control

* Github create a release

# Base functionality

* Add macros l_until(cond), l_until(int), l_when(cond), l_regularly(time), l_regularly(int)
* Redirect all logging to a configurable stream. Make the disable functionality redirect it to no stream.