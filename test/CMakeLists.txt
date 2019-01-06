cmake_minimum_required(VERSION 3.6)
project(test)

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

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++ -Wshadow")
endif()

include_directories(include)
include_directories(../include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(libtest src/test.cpp)

add_executable(d        src/d.cpp)
add_executable(d_arr    src/d_arr.cpp)
add_executable(d_mat    src/d_mat.cpp)
add_executable(disabled src/disabled.cpp)
add_executable(postfix  src/postfix.cpp)
add_executable(prefixes src/prefixes.cpp)
add_executable(run_all  src/run_all.cpp)

if(CLANG_TIDY_EXE)
    set_target_properties(
        libtest PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        d PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        d_arr PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        d_mat PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
    set_target_properties(
        disabled PROPERTIES
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

target_link_libraries(d        libtest)
target_link_libraries(d_arr    libtest)
target_link_libraries(d_mat    libtest)
target_link_libraries(disabled libtest)
target_link_libraries(postfix  libtest)
target_link_libraries(prefixes libtest)