cmake_minimum_required(VERSION 3.6)
project(goinglogging_test)

option(BUILD_DOC "Build documentation" ON)

message(STATUS "Generator: ${CMAKE_GENERATOR}")

find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-fuchsia-default-arguments,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay,-fuchsia-overloaded-operator,-cert-env33-c")
endif()
find_package(Doxygen)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++ -Wshadow")
endif()

include_directories(include)
include_directories(../include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(libtest src/test.cpp)

add_executable(c_types        src/c_types.cpp)
add_executable(color          src/color.cpp)
add_executable(cpp_types      src/cpp_types.cpp)
add_executable(custom         src/custom.cpp)
add_executable(l              src/l.cpp)
add_executable(l_arr          src/l_arr.cpp)
add_executable(l_mat          src/l_mat.cpp)
add_executable(output_enabled src/output_enabled.cpp)
add_executable(postfix        src/postfix.cpp)
add_executable(prefixes       src/prefixes.cpp)
add_executable(run_all        src/run_all.cpp)

if(DO_CLANG_TIDY)
    set_target_properties(
        libtest PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        c_types PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        color PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        cpp_types PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        custom PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        l PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        l_arr PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        l_mat PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        output_enabled PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        postfix PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        prefixes PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        run_all PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
endif()

target_link_libraries(c_types        libtest)
target_link_libraries(color          libtest)
target_link_libraries(cpp_types      libtest)
target_link_libraries(custom         libtest)
target_link_libraries(l              libtest)
target_link_libraries(l_arr          libtest)
target_link_libraries(l_mat          libtest)
target_link_libraries(output_enabled libtest)
target_link_libraries(postfix        libtest)
target_link_libraries(prefixes       libtest)

# From https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
if(BUILD_DOC AND DOXYGEN_FOUND)
    # set input and output files
    message(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Testing framework doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen_test ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating testing framework API documentation with Doxygen"
        VERBATIM)
else()
  message("Doxygen need to be installed to generate doxygen documentation")
endif()