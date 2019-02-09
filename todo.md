# Cleanup

## Tools

* Static analysis
* Compile with all warnings enabled
* Valgrind
* Cmake check if __cxa_demangle exists

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
* Ensure almost no usage of std::cout
* Use ::gl::internal instead of gl::internal
* Fix order of functions and classes
* Pass std::string by reference only when necessary
* Don't convert between const char* and std::string too much
* Don't use C-style comments
* Ensure parameters are ordered nicely
* Include what's used but nothing more
* Make functions that can be static static
* Break out conversion to UTF-8 to function
* Why do some functions need to use get_value()?
* Add advanced test, such as std::map<std::pair<int, std::string>, std::vector<float>>
* namespace testing framework

# Features

## External libraries

* Rip out cxx-pretty-print if permissible by license
* Rip out type name library?

## Testing

* Docker framework

## VS code

* Build with CMake from IDE
* Launch binary from IDE

## Documentation

* Add problem description (printf("i = %s\n"); vs l(i);)
* Better looking doxygen theme

## Build system

* Run doxygen in cmake for both main library and test
* ninja install

## Version control

* Github create a release

## Base functionality

* Add macros l_until(cond), l_until(int), l_when(cond), l_regularly(time), l_regularly(int)
* Redirect all logging to a configurable stream. Make the disable functionality redirect it to no stream.

## Error handling

* Add, use and document exception type
* Give a useful error message when operator<< isn't overloaded

## C++ > 14

* std::string_view