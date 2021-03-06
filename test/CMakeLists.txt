# Setup
cmake_minimum_required(VERSION 3.6)
project(goinglogging_test)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../")

# Options
option(BUILD_DOC "Build documentation" ON)

# Info
message(STATUS "Generator: ${CMAKE_GENERATOR}")

# Check existence of programs and libraries
find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-fuchsia-default-arguments,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay,-fuchsia-overloaded-operator,-cert-env33-c")
endif()
find_package(Doxygen)
find_package(Breathe)
find_package(Sphinx)

# Include directories
include_directories(include)
include_directories(../include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# All executables
set(executables
    "src/c_types.cpp"
    "src/color.cpp"
    "src/cpp_types.cpp"
    "src/custom.cpp"
    "src/l.cpp"
    "src/l_arr.cpp"
    "src/l_mat.cpp"
    "src/output_enabled.cpp"
    "src/postfix.cpp"
    "src/prefixes.cpp"
    "src/run_all.cpp"
)

# Add libraries
add_library(libtest src/test.cpp)

# Add executables
foreach(file ${executables})
  # Convert filename to executable name
  get_filename_component(exe ${file} NAME_WE)
  # Add
  add_executable(${exe} ${file})
  # Use static analysis if enabled
  if(CLANG_TIDY_EXE)
    set_target_properties(
        ${exe} PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
  endif()
  # Link to library
  target_link_libraries(${exe} libtest)
endforeach()

# Enable compiler specific warnings
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++ -Wshadow")
endif()

if(BUILD_DOC AND DOXYGEN_FOUND AND BREATHE_FOUND AND SPHINX_FOUND)
    # set input and output files
    message(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Testing framework doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating testing framework API documentation with Doxygen"
        VERBATIM)

    if(NOT DEFINED SPHINX_THEME)
        set(SPHINX_THEME default)
    endif()

    if(NOT DEFINED SPHINX_THEME_DIR)
        set(SPHINX_THEME_DIR)
    endif()

    # Configured documentation tools and intermediate build results
    set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

    # Sphinx cache with pickled ReST documents
    set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

    # HTML output directory
    set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

    configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
    "${BINARY_BUILD_DIR}/conf.py"
    @ONLY)

    add_custom_target(sphinx ALL
        ${SPHINX_EXECUTABLE}
            -q -b html
            -c "${BINARY_BUILD_DIR}"
            -d "${SPHINX_CACHE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${SPHINX_HTML_DIR}"
        COMMENT "Building HTML documentation with Sphinx" VERBATIM)
else()
  message("Doxygen need to be installed to generate doxygen documentation")
endif()
