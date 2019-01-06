/** \mainpage
 * \brief Lightweight logging library.
 *
 * \section section_description Description
 *
 * \e goinglogging is an open source, lightweight, multiplatform, single header 
 * C++11 library that logs information to command line or file. This is useful 
 * when debugging realtime systems where compiling with debug symbols and/or a
 * low optimization level will slow down the program too much. This library is
 * designed to make it as effortless as possible to output the name and value
 * of a variable. 
 *
 * \note goinglogging is a logging library for debugging when a debugger is a
 * worse alternative. It is not for heavy logging. Use a proper library for
 * that.
 *
 * goinglogging is MIT-licensed.
 *
 * \author Emil Berg
 *
 * \section section_usage Usage
 *
 * \code
 * #include "goinglogging/goinglogging.h"
 * \endcode
 *
 * \subsection section_a_variable A variable
 * \code
 * int i = 1;
 * D(i);
 * \endcode
 * Which outputs:
 * \code
 * i: 1
 * \endcode
 * \sa D()
 *
 * \subsection section_multiple_variables Multiple variables
 * \code
 * int i = 1;
 * const char* s = "s";
 * D(i, s);
 * \endcode
 * Which outputs:
 * \code
 * i: 1, s: s
 * \endcode
 * \sa D()
 *
 * \subsection section_array Array
 * \code
 * int i[] = {0, 1, 2};
 * D_ARR(i, 3);
 * \endcode
 * \sa D_ARR()
 *
 * \subsection section_matrix Matrix
 * \code
 * int i[][] = {{0, 11}, {22, 33}};
 * D_MAT(i, 2, 2);
 * \endcode
 * \sa D_MAT()
 *
 * \subsection section_custom_objects Custom objects
 * Can output any object with an overloaded << operator.
 *
 * \subsection section_prefixes Prefixes
 * Output file, line, and other information by using:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * int i = 1;
 * D(i);
 * \endcode
 * Which outputs:
 * \code
 * my_file:68: i: 1
 * \endcode
 * \sa set_prefixes()
 *
 * \subsection section_disabled_output Disable output
 * \code
 * #define GOINGLOGGING_DISABLED
 * #include "goinglogging/goinglogging.h"
 * \endcode
 *
 * \subsection section_flush_output Flush output
 * goinglogging will not flush output by default. To ensure it flushes, use:
 * \code
 * #define GOINGLOGGING_NEWLINE std::endl
 * #include "goinglogging/goinglogging.h"
 * \endcode
 *
 * \subsection section_redirect Redirect
 * Redirect output to file:
 * \code
 * std::ofstream f("f.txt");
 * std::cout.rdbuf(f.rdbuf());
 * \endcode
 *
 */

/** \file
 *
 * \brief Lightweight logging library.
 *
 */

#ifndef INCLUDE_GOINGLOGGING_H_
#define INCLUDE_GOINGLOGGING_H_

#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifndef GOINGLOGGING_NEWLINE
/** \brief Newline character to use after each logging message.
 *
 * May e.g. be changed to std::endl to enable flush after each
 * logging message:
 * \code
 * #define GOINGLOGGING_NEWLINE std::endl
 * #include "goinglogging/goinglogging.h"
 * \endcode
 *
 * \note Defaults to \\n, i.e. no flushing.
 *
 */
#define GOINGLOGGING_NEWLINE '\n'
#endif  // GOINGLOGGING_NEWLINE

/** \brief Namespace for goinglogging library  */
namespace gl {

/** \brief Prefix of logging output.
 *
 * Use bitwise \c or for multiple prefixes at the same time:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * \endcode
 * Bitwise \c and as well as \c xor are also supported.
 *
 * \sa set_prefix()
 *
 */
enum class prefix : uint32_t {
    NONE     = 0,    /**< No prefix. */
    FILE     = 1<<0, /**< File name. For example 'main.cpp'. */
    LINE     = 1<<1, /**< Line number in file. For example 'Line: 16'. */
    FUNCTION = 1<<2, /**< Function name. For example 'calculate()'. */
    TIME     = 1<<3, /**< Current local time as hour:minute:second. For example '10:22:13'. */
    THREAD   = 1<<4  /**< ID of current thread. For example 'TID: 12'. */
};

/** \brief Bitwise \c and of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c and of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix operator&(prefix lhs, prefix rhs) noexcept {
    return static_cast<prefix>(static_cast<uint32_t>(lhs) &
                               static_cast<uint32_t>(rhs));
}

/** \brief Bitwise \c and of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c and of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix& operator&=(prefix& lhs, prefix rhs) noexcept {
    return lhs = lhs & rhs;
}

/** \brief Bitwise \c or of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c or of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix operator|(prefix lhs, prefix rhs) noexcept {
    return static_cast<prefix>(static_cast<uint32_t>(lhs) |
                               static_cast<uint32_t>(rhs));
}

/** \brief Bitwise \c or of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c or of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix& operator|=(prefix& lhs, prefix rhs) noexcept {
    return lhs = lhs | rhs;
}

/** \brief Bitwise \c xor of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c xor of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix operator^(prefix lhs, prefix rhs) noexcept {
    return static_cast<prefix>(static_cast<uint32_t>(lhs) ^
                               static_cast<uint32_t>(rhs));
}

/** \brief Bitwise \c xor of logging prefix settings.
 *
 * \param lhs First element.
 * \param rhs Second element.
 * \return Bitwise \c xor of \p lhs and \p rhs.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
inline prefix& operator^=(prefix& lhs, prefix rhs) noexcept {
    return lhs = lhs ^ rhs;
}

/* Hide from doxygen */
#ifndef DOXYGEN_HIDDEN

/** \brief Functionality in this namespace is for internal use */
namespace internal {

static prefix curPrefixes = prefix::NONE; /**< \brief Current prefixes */

/** \brief Print prefix string.
 *
 * \note For internal use.
 * \param file File name
 * \param line Line in file
 * \param func Function name
 *
 */
void print_prefix(const std::string& file, unsigned int line,
                  const std::string& func) noexcept {
#ifndef GOINGLOGGING_DISABLED
    uint32_t cnt = 0;

    if ((curPrefixes & prefix::FILE) != prefix::NONE) {
        const char sep =
#ifdef _WIN32
            '\\';
#else
            '/';
#endif  // _WIN32
        size_t idx = file.find_last_of(sep);
        std::cout << (idx == std::string::npos ? file : file.substr(idx + 1));
        ++cnt;
    }
    if ((curPrefixes & prefix::LINE) != prefix::NONE) {
        if (cnt == 0) {
            std::cout << "Line: ";
        } else {
            std::cout << ":";
        }
        std::cout << line;
        ++cnt;
    }
    if ((curPrefixes & prefix::FUNCTION) != prefix::NONE) {
        if (cnt != 0) {
            std::cout << ", ";
        }
        std::cout << func << "()";
        ++cnt;
    }
    if ((curPrefixes & prefix::TIME) != prefix::NONE) {
        if (cnt != 0) {
            std::cout << ", ";
        }
        auto t = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(t);
        std::cout << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
        ++cnt;
    }
    if ((curPrefixes & prefix::THREAD) != prefix::NONE) {
        if (cnt != 0) {
            std::cout << ", ";
        }
        std::cout << "TID: " << std::this_thread::get_id();
        ++cnt;
    }

    if (cnt != 0) {
        std::cout << ": ";
    }
#else
    // Avoid warning of unused variable
    (void)(file);
    (void)(line);
    (void)(func);
#endif  // GOINGLOGGING_DISABLED
}

/** \brief Log array.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param name Variable name.
 * \param file File name.
 * \param line File line number.
 * \param func Function name.
 * \param v    Variable values.
 * \param n    Number of elements.
 *
 */
template<class T>
void array(const char* name, const char* file, unsigned int line,
           const char* func, const T v, size_t n) noexcept {
#ifndef GOINGLOGGING_DISABLED
    print_prefix(file, line, func);
    std::cout << name << ": ";
    if (n <= 0) {
        std::cout << "{}";
    } else {
        std::cout << "[0]: " << v[0];
        for (size_t i = 1; i < n; ++i) {
            std::cout << ", [" << i << "]: " << v[i];
        }
    }
    std::cout << GOINGLOGGING_NEWLINE;
#else
    // Avoid warning of unused variables
    (void)(name);
    (void)(file);
    (void)(line);
    (void)(func);
    (void)(v);
    (void)(n);
#endif  // GOINGLOGGING_DISABLED
}

/** \brief Log matrix with dimensions \p c * \p r.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param name Variable name.
 * \param file File name.
 * \param line File line number.
 * \param func Function name.
 * \param m    Variable values.
 * \param c    Number of columns.
 * \param r    Number of rows.
 *
 */
template<class T>
void matrix(const char* name, const char* file, unsigned int line,
            const char* func, const T m, size_t c, size_t r) noexcept {
#ifndef GOINGLOGGING_DISABLED
    print_prefix(file, line, func);
    std::cout << name << ": ";
    if (c <= 0 || r <= 0) {
        std::cout << "{}";
    } else {
        std::cout << "[0,0]: " << m[0][0];
        for (size_t j = 1; j < c; ++j) {
            std::cout << ", " << "[0," << j << "]: " << m[0][j];
        }
        for (size_t i = 1; i < r; ++i) {
            for (size_t j = 0; j < c; ++j) {
                std::cout << ", " << "[" << i << ',' << j << "]: " << m[i][j];
            }
        }
    }
    std::cout << GOINGLOGGING_NEWLINE;
#else
    // Avoid warnings of unused variables
    (void)(name);
    (void)(file);
    (void)(line);
    (void)(func);
    (void)(m);
    (void)(c);
    (void)(r);
#endif  // GOINGLOGGING_DISABLED
}

}  // namespace internal

#endif  // DOXYGEN_HIDDEN

/** \brief Get prefixes of logging output.
 *
 * \return Prefix settings.
 *
 * \sa prefix \sa get_prefixes()
 *
 */
prefix get_prefixes() noexcept {
    return internal::curPrefixes;
}

/** \brief Set prefixes of logging output.
 *
 * Alters the output of \sa D(), \sa D_ARR(), and \sa D_MAT().
 *
 * \param p Bitwise \c or of prefix settings.
 *
 * Used as:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * \endcode
 * to also log current file and line number.
 *
 * \note Defaults to prefix::NONE.
 *
 * \sa prefix \sa get_prefixes()
 *
 */
void set_prefixes(prefix p) noexcept {
    internal::curPrefixes = p;
}

#ifndef DOXYGEN_HIDDEN
#define GOINGLOGGING_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, \
    _12, _13, _14, _15, _16, NAME, ...) NAME
#endif  // DOXYGEN_HIDDEN

#ifndef GOINGLOGGING_DISABLED
/** \brief Log variables to stdout.
 *
 * Use as:
 * \code
 * int i = 1;
 * D(i);
 * std::string s("s");
 * D(i, s);
 * \endcode
 * Which outputs:
 * \code
 * i: 1, s: s
 * \endcode
 *
 * \note Supports up to 16 variables on the same line.
 * \note Uses prefix information set with \ref set_prefixes().
 *
 * \sa D_ARR() \sa D_MAT() \sa set_prefixes().
 *
 */
#define D(...) do { \
gl::internal::print_prefix(__FILE__, __LINE__, __func__); \
GOINGLOGGING_GET_MACRO(__VA_ARGS__, GOINGLOGGING16, GOINGLOGGING15, \
    GOINGLOGGING14, GOINGLOGGING13, GOINGLOGGING12, GOINGLOGGING11, \
    GOINGLOGGING10, GOINGLOGGING9, GOINGLOGGING8, GOINGLOGGING7, \
    GOINGLOGGING6, GOINGLOGGING5, GOINGLOGGING4, GOINGLOGGING3, \
    GOINGLOGGING2, GOINGLOGGING1,)(__VA_ARGS__) << (GOINGLOGGING_NEWLINE); \
} while (false)
#else
#define D(...) do { \
gl::internal::print_prefix(__FILE__, __LINE__, __func__); \
    GOINGLOGGING_GET_MACRO(__VA_ARGS__, GOINGLOGGING16, GOINGLOGGING15, \
    GOINGLOGGING14, GOINGLOGGING13, GOINGLOGGING12, GOINGLOGGING11, \
    GOINGLOGGING10, GOINGLOGGING9, GOINGLOGGING8, GOINGLOGGING7, \
    GOINGLOGGING6, GOINGLOGGING5, GOINGLOGGING4, GOINGLOGGING3, GOINGLOGGING2, \
    GOINGLOGGING1,)(__VA_ARGS__) \
} while (false)
#endif  // GOINGLOGGING_DISABLED

#ifndef DOXYGEN_HIDDEN

#ifndef GOINGLOGGING_DISABLED

#define GOINGLOGGING1(v1) \
    std::cout << (#v1) << ": " << (v1)

#define GOINGLOGGING2(v1, v2) \
    GOINGLOGGING1(v1) << ", " << (#v2) << ": " << (v2)

#define GOINGLOGGING3(v1, v2, v3) \
    GOINGLOGGING2(v1, v2) << ", " << (#v3) << ": " << (v3)

#define GOINGLOGGING4(v1, v2, v3, v4) \
    GOINGLOGGING3(v1, v2, v3) << ", " << (#v4) << ": " << (v4)

#define GOINGLOGGING5(v1, v2, v3, v4, v5) \
    GOINGLOGGING4(v1, v2, v3, v4) << ", " << (#v5) << ": " << (v5)

#define GOINGLOGGING6(v1, v2, v3, v4, v5, v6) \
    GOINGLOGGING5(v1, v2, v3, v4, v5) << ", " << (#v6) << ": " << (v6)

#define GOINGLOGGING7(v1, v2, v3, v4, v5, v6, v7) \
    GOINGLOGGING6(v1, v2, v3, v4, v5, v6) << ", " << (#v7) << ": " << (v7)

#define GOINGLOGGING8(v1, v2, v3, v4, v5, v6, v7, v8) \
    GOINGLOGGING7(v1, v2, v3, v4, v5, v6, v7) << ", " << (#v8) << ": " << (v8)

#define GOINGLOGGING9(v1, v2, v3, v4, v5, v6, v7, v8, v9) \
    GOINGLOGGING8(v1, v2, v3, v4, v5, v6, v7, v8) << \
        ", " << (#v9) << ": " << (v9)

#define GOINGLOGGING10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
    GOINGLOGGING9(v1, v2, v3, v4, v5, v6, v7, v8, v9) << \
        ", " << (#v10) << ": " << (v10)

#define GOINGLOGGING11(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
    GOINGLOGGING10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) << \
        ", " << (#v11) << ": " << (v11)

#define GOINGLOGGING12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
    GOINGLOGGING11(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) << \
        ", " << (#v12) << ": " << (v12)

#define GOINGLOGGING13(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                       v13) \
    GOINGLOGGING12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) << \
        ", " << (#v13) << ": " << (v13)

#define GOINGLOGGING14(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                       v13, v14) \
    GOINGLOGGING13(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) << \
        ", " << (#v14) << ": " << (v14)

#define GOINGLOGGING15(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                       v13, v14, v15) \
    GOINGLOGGING14(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, \
                   v14) << \
        ", " << (#v15) << ": " << (v15)

#define GOINGLOGGING16(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                       v13, v14, v15, v16) \
    GOINGLOGGING15(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, \
                   v14, v15) << \
        ", " << (#v16) << ": " << (v16)

#else

#define GOINGLOGGING1(v1) \
    (void)(v1);

#define GOINGLOGGING2(v1, v2) \
    GOINGLOGGING1(v1); (void)(v2);

#define GOINGLOGGING3(v1, v2, v3) \
    GOINGLOGGING2(v1); (void)(v3);

#define GOINGLOGGING4(v1, v2, v3, v4) \
    GOINGLOGGING3(v1); (void)(v4);

#define GOINGLOGGING5(v1, v2, v3, v4, v5) \
    GOINGLOGGING4(v1); (void)(v5);

#define GOINGLOGGING6(v1, v2, v3, v4, v5, v6) \
    GOINGLOGGING5(v1); (void)(v6);

#define GOINGLOGGING7(v1, v2, v3, v4, v5, v6, v7) \
    GOINGLOGGING6(v1); (void)(v7);

#define GOINGLOGGING8(v1, v2, v3, v4, v5, v6, v7, v8) \
    GOINGLOGGING7(v1); (void)(v8);

#define GOINGLOGGING9(v1, v2, v3, v4, v5, v6, v7, v8, v9) \
    GOINGLOGGING8(v1); (void)(v9);

#define GOINGLOGGING10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
    GOINGLOGGING9(v1); (void)(v10);

#define GOINGLOGGING11(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
    GOINGLOGGING10(v1); (void)(v11);

#define GOINGLOGGING12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
    GOINGLOGGING11(v1); (void)(v12);

#define GOINGLOGGING13(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
    GOINGLOGGING12(v1); (void)(v13);

#define GOINGLOGGING14(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, \
                       v14) \
    GOINGLOGGING13(v1); (void)(v14);

#define GOINGLOGGING15(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, \
                       v14, v15) \
    GOINGLOGGING14(v1); (void)(v15);

#define GOINGLOGGING16(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, \
                       v14, v15, v16) \
    GOINGLOGGING15(v1); (void)(v16);

#endif  // GOINGLOGGING_DISABLED

#endif  // DOXYGEN_HIDDEN

/** \brief Log array to stdout.
 *
 * \param v Array to print.
 * \param n Number of elements.
 *
 * Used as:
 * \code
 * int a[] = {3, 4, 5};
 * D_ARR(a, 3);
 * \endcode
 *
 * Which outputs:
 * \code
 * a: [0]: 3, [1]: 4, [2]: 5
 * \endcode
 *
 * \note Uses prefix information set with \ref set_prefixes().
 *
 * \warning Behaviour is undefined if \p n is larger than the allocated array.
 *
 * \sa D() \sa D_MAT() \sa set_prefixes()
 *
 */
#define D_ARR(v, n) \
    do { \
        gl::internal::array((#v), __FILE__, __LINE__, __func__, (v), (n)); \
    } while (false)

/** \brief Log matrix to stdout.
 *
 * \param m Matrix to print.
 * \param c Number of columns in matrix.
 * \param r Number of rows in matrix.
 *
 * Used as:
 * \code
 * int m[][] = {{11, 12}, {21, 22}};
 * D_MAT(m, 2, 2);
 * \endcode
 *
 * Which outputs:
 * \code
 * m: [0,0]: 11, [0,1]: 12, [1,0]: 21, [1,1]: 22
 * \endcode
 *
 * \note Uses prefix information set with \ref set_prefixes()
 *
 * \warning Behaviour is undefined if \p c or \p r is larger than the
 *          allocated matrix.
 *
 * \sa D() \sa D_ARR() \sa set_prefixes()
 *
 */
#define D_MAT(m, c, r) \
    do { \
        gl::internal::matrix((#m), __FILE__, __LINE__, __func__, (m), (c), \
                             (r)); \
    } while (false)

} // namespace gl

#endif  // INCLUDE_GOINGLOGGING_H_