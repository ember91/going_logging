# Cleanup

## Tools

* Static analysis
* Compile with all warnings enabled
* Valgrind
* Clean up CMakeLists.txt

## Testing

* Test on Windows
* Control regexes in test ground truth

## Documentation

* Improve
* Neither getters nor setters should use \brief
* Ensure template parameters are documented with \tparam
* Ensure relevant features are listed in both doxygen and README
* Ensure documentation such as \param ends with dot

## Consistency

* Add noexcept, const, constepxr, explicit to functions
* Don't overuse noexcept
* Add const to parameters and members where applicable
* Forward declare all functions
* Ensure classes are PascalCase, variables camelCase and functions snake_case
* Inline functions in classes?
* Use C++11 functionality
* Ensure one character c-strings use single quotes
* Ensure almost no usage of std::cout
* Use ::gl::internal instead of gl::internal
* Fix order of functions and classes
* Pass std::string by reference only when necessary
* Don't convert between const char* and std::string too much
* Don't use C-style comments
* Ensure parameters are in a good order
* Include what's used but nothing more
* Make functions that can be static static

# Features

## Data type prefix

* Readd functionality for this with __cxa_demangle in GCC, clang

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

## Base functionality

* Add macros l_until(cond), l_until(int), l_when(cond), l_regularly(time), l_regularly(int)
* Redirect all logging to a configurable stream. Make the disable functionality redirect it to no stream.

## Error handling

* Add and use exception type