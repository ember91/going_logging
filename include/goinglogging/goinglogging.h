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
 * l(i);
 * \endcode
 * Which outputs:
 * \code
 * i = 1
 * \endcode
 * \sa l()
 *
 * \subsection section_multiple_variables Multiple variables
 * \code
 * int i = 1;
 * const char* s = "s";
 * l(i, s);
 * \endcode
 * Which outputs:
 * \code
 * i = 1, s = "s"
 * \endcode
 * \sa l()
 *
 * \subsection section_array Array
 * \code
 * int i[] = {0, 1, 2};
 * l_arr(i, 3);
 * \endcode
 * \sa l_arr()
 *
 * \subsection section_matrix Matrix
 * \code
 * int i[][] = {{0, 11}, {22, 33}};
 * l_mat(i, 2, 2);
 * \endcode
 * \sa l_mat()
 *
 * \subsection section_custom_objects Custom objects
 * Can output any object with an overloaded << operator.
 *
 * \subsection section_prefixes Prefixes
 * Output file, line, and other information by using:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * int i = 1;
 * l(i);
 * \endcode
 * Which outputs:
 * \code
 * my_file:68: i = 1
 * \endcode
 * \sa set_prefixes()
 *
 * \subsection section_disabled_output Disable output
 * \code
 * gl::set_output_enabled(false);
 * \endcode
 * \sa set_output_enabled()
 *
 * \subsection section_flush_output Flush output
 * goinglogging will not flush output by default. To ensure it flushes, use:
 * \code
 * #define GL_NEWLINE std::endl
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
 * \subsection section_color Color
 * Enable colored output in terminals that support ANSI control sequences:
 * \code
 * gl::set_color_enabled(true);
 * \endcode
 * \sa set_color_enabled()
 *
 */

/** \file
 *
 * \brief Lightweight logging library.
 *
 */

#ifndef INCLUDE_GOINGLOGGING_H_
#define INCLUDE_GOINGLOGGING_H_

#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <deque>
#include <forward_list>
#include <iomanip>
#include <ios>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/** \brief Log variables.
 *
 * Use as:
 * \code
 * int i = 1;
 * l(i);
 * std::string s("s");
 * l(i, s);
 * \endcode
 * Which outputs:
 * \code
 * i = 1, s = "s"
 * \endcode
 *
 * \note Supports up to 16 variables as parameters.
 * \note Uses prefix information set with \ref set_prefixes().
 *
 * \sa l_arr() \sa l_mat() \sa set_prefixes()
 *
 */
#define l(...)                                                                 \
    do {                                                                       \
        if (::gl::internal::outputEnabled) {                                   \
            std::cout << gl::internal::color_start                             \
                      << gl::internal::PrefixFormatter(                        \
                             __FILE__, __LINE__, __func__)                     \
                      << GL_INTERNAL_L_DISPATCH(__VA_ARGS__, GL_INTERNAL_L16,  \
                             GL_INTERNAL_L15, GL_INTERNAL_L14,                 \
                             GL_INTERNAL_L13, GL_INTERNAL_L12,                 \
                             GL_INTERNAL_L11, GL_INTERNAL_L10, GL_INTERNAL_L9, \
                             GL_INTERNAL_L8, GL_INTERNAL_L7, GL_INTERNAL_L6,   \
                             GL_INTERNAL_L5, GL_INTERNAL_L4, GL_INTERNAL_L3,   \
                             GL_INTERNAL_L2, GL_INTERNAL_L1, )(__VA_ARGS__)    \
                      << gl::internal::color_end << (GL_NEWLINE);              \
        }                                                                      \
    } while (false)

/** \brief Log array.
 *
 * \param v   Array to print [\p len].
 * \param len Number of elements.
 *
 * Used as:
 * \code
 * int a[] = {0, 1, 2};
 * l_arr(a, 3);
 * \endcode
 *
 * Which outputs:
 * \code
 * a = {0, 1, 2}
 * \endcode
 *
 * \note Uses prefix information set with \ref set_prefixes().
 * \note Unlike \ref l(), this does not support multiple varaibles as
 * parameters.
 *
 * \warning Behaviour is undefined if \p len is larger than the
 * allocated array.
 *
 * \sa l() \sa l_mat() \sa set_prefixes()
 *
 */
#define l_arr(v, len)                                                       \
    do {                                                                    \
        std::cout << ::gl::internal::make_array((#v), (v), (len),           \
            ::gl::internal::PrefixFormatter(__FILE__, __LINE__, __func__)); \
    } while (false)

/** \brief Log matrix.
 *
 * \param m    Matrix to print [\p rows x \p cols].
 * \param cols Number of columns in matrix.
 * \param rows Number of rows in matrix.
 *
 * Used as:
 * \code
 * int m[][] = {{11, 12}, {21, 22}};
 * l_mat(m, 2, 2);
 * \endcode
 *
 * Which outputs:
 * \code
 * m: [0,0] = 11, [0,1] = 12, [1,0] = 21, [1,1] = 22
 * \endcode
 *
 * \note Uses prefix information set with \ref set_prefixes()
 * \note Unlike \ref l(), this does not support multiple variables
 * as parameters.
 *
 * \warning Behaviour is undefined if \p c or \p r is larger than
 * the allocated matrix.
 *
 * \sa l() \sa l_arr() \sa set_prefixes()
 *
 */
#define l_mat(m, cols, rows)                                                \
    do {                                                                    \
        std::cout << ::gl::internal::make_matrix((#m), (m), (cols), (rows), \
            ::gl::internal::PrefixFormatter(__FILE__, __LINE__, __func__)); \
    } while (false)

#ifndef GL_NEWLINE
/** \brief Newline character to use after each logging message.
 *
 * May e.g. be changed to std::endl to enable flush after each
 * logging message:
 * \code
 * #define GL_NEWLINE std::endl
 * #include "goinglogging/goinglogging.h"
 * \endcode
 *
 * \note Defaults to \\n, i.e. no flushing.
 *
 * \sa l() \sa l_arr() \sa l_mat()
 *
 */
#define GL_NEWLINE '\n'
#endif // GL_NEWLINE

/** \brief goinglogging namespace. */
namespace gl {

/** \brief Prefix of logging output.
 *
 * Use bitwise \c or for multiple prefixes at the same time:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * \endcode
 * Bitwise \c and as well as \c xor are also supported.
 *
 * \sa set_prefixes() \sa get_prefixes()
 *
 */
enum class prefix : uint32_t {
    NONE = 0,          /**< No prefix. */
    FILE = 1 << 0,     /**< File name. For example 'main.cpp'. */
    LINE = 1 << 1,     /**< Line number in file. For example 'Line:
                          16'. */
    FUNCTION = 1 << 2, /**< Function name. For example 'calculate()'. */
    TIME     = 1 << 3, /**< Current local time as
                        * hour:minute:second.millisecond.
                        *  For example '10:02:13.057'. */
    THREAD = 1 << 4,   /**< ID of current thread. For example
                          'TID:   12'. */
    TYPE_NAME = 1 << 5 /**< Name of type. For example 'int'. */
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
    return static_cast<prefix>(
        static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
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
    return static_cast<prefix>(
        static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
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
    return static_cast<prefix>(
        static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
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

/** \brief Hide this section from doxygen */
#ifndef DOXYGEN_HIDDEN

/** \brief Functionality in this namespace is for internal use
 */
namespace internal {

/** Current prefixes */
static prefix curPrefixes = prefix::FILE | prefix::LINE;
/** \c true if output is enabled */
static bool outputEnabled = true;
/** \c true if colored output is enabled */
static bool colorEnabled = false;

/** \brief Prefix formatter. */
class PrefixFormatter {
  public:
    /** \brief Construct.
     *
     * \param file_path File path including name.
     * \param file_line Line number in file.
     * \param func      Function name.
     *
     */
    PrefixFormatter(
        const char* file_path, long file_line, const char* func) noexcept :
        m_file_path(file_path),
        m_file_line(file_line), m_func(func) {
    }

    friend std::ostream& operator<<(
        std::ostream& os, const PrefixFormatter& p) noexcept;

    /**
     * \return File path including name.
     */
    const char* get_file_path() const noexcept {
        return m_file_path;
    }

    /**
     * \return Line number in file.
     */
    long get_file_line_number() const noexcept {
        return m_file_line;
    }

    /**
     * \return Function name.
     */
    const char* get_function_name() const noexcept {
        return m_func;
    }

  private:
    const char* m_file_path; /**< File path including name. */
    const long  m_file_line; /**< Line number in file. */
    const char* m_func;      /**< Function name. */
};

/** \brief Write to stream.
 *
 * \param os Output stream.
 * \param p  PrefixFormatter to output.
 * \return Output stream.
 *
 */
std::ostream& operator<<(std::ostream& os, const PrefixFormatter& p) noexcept {
    /** Number of prefixes written */
    uint32_t cnt = 0;

    // FILE
    if ((curPrefixes & prefix::FILE) != prefix::NONE) {
        /** Platform dependent path separator */
        const char sep =
#ifdef _WIN32
            '\\';
#else
            '/';
#endif // _WIN32

        /** File path */
        const char* path = p.get_file_path();

        // Get index of last file path separator
        /** Length of string */
        size_t str_len = std::strlen(path);
        /** File path separator index */
        size_t sep_idx = std::numeric_limits<size_t>::max();
        // No need to check for str_len = 0 here, since that
        // will stop the for loop anyway
        for (size_t i = str_len - 1; i != std::numeric_limits<size_t>::max();
             --i) {
            if (path[i] == sep) {
                sep_idx = i;
                break;
            }
        }

        // Output
        os << (sep_idx == std::numeric_limits<size_t>::max() ?
                   path :
                   path + sep_idx + 1);

        ++cnt;
    }

    // LINE
    if ((curPrefixes & prefix::LINE) != prefix::NONE) {
        // Output 'Line' prefix only if file name has not been
        // output
        if (cnt == 0) {
            os << "Line: ";
        } else {
            os << ":";
        }
        os << p.get_file_line_number();

        ++cnt;
    }

    // FUNCTION
    if ((curPrefixes & prefix::FUNCTION) != prefix::NONE) {
        if (cnt != 0) {
            os << ", ";
        }
        os << p.get_function_name() << "()";
        ++cnt;
    }

    // TIME
    if ((curPrefixes & prefix::TIME) != prefix::NONE) {
        // Get time
        auto now = std::chrono::system_clock::now();

        // Convert
        std::time_t sinceEpoch = std::chrono::system_clock::to_time_t(now);
        auto        ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;
        std::tm* local = std::localtime(&sinceEpoch);

        if (local != nullptr) {
            if (cnt != 0) {
                os << ", ";
            }
            // Format and output
            os << std::put_time(local, "%H:%M:%S") << '.' << std::setfill('0')
               << std::setw(3) << ms.count();
            ++cnt;
        }
    }

    // THREAD
    if ((curPrefixes & prefix::THREAD) != prefix::NONE) {
        if (cnt != 0) {
            os << ", ";
        }
        os << "TID: " << std::this_thread::get_id();
        ++cnt;
    }

    // Final separator, if any
    if (cnt != 0) {
        os << ": ";
    }

    return os;
}

/** \brief Variable value formatter.
 *
 * \tparam T Value type.
 *
 */
template<class T>
class ValueFormatter {
  public:
    /** \brief Construct.
     *
     * \param val Value.
     *
     */
    explicit ValueFormatter(T& val) noexcept : m_val(val) {
    }

    // Container value helper functions.
    std::ostream& sequence(std::ostream& os) const noexcept;
    std::ostream& map(std::ostream& os) const noexcept;
    std::ostream& stack(std::ostream& os) const noexcept;
    std::ostream& queue(std::ostream& os) const noexcept;

    template<class U>
    friend std::ostream& operator<<(
        std::ostream& os, const ValueFormatter<U>& f) noexcept;

  private:
    T& m_val; /**< Value to format. Use non-const reference in case operator<<
                 isn't const. */
};

/** \brief Create ValueFormatter with the right template type.
 *  This solves the error "cannot refer to class template
 * 'ValueFormatter' without a template argument list".
 *
 * \tparam T Value type.
 * \param val Value.
 * \return New ValueFormatter.
 */
template<class T>
ValueFormatter<T> format_value(T& val) {
    return ValueFormatter<T>(val);
};

/** \brief Write a sequence, defined by begin() and end(), to stream.
 *
 * \tparam T Value type.
 * \param os Output stream.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& ValueFormatter<T>::sequence(std::ostream& os) const noexcept {
    os << '{';
    // Print first object without comma
    auto it = m_val.begin();
    if (it != m_val.end()) {
        os << format_value(*it);
        ++it;
    }
    // Print the rest
    for (; it != m_val.end(); ++it) {
        os << ", " << format_value(*it);
    }
    os << '}';

    return os;
}

/** \brief Write a map, defined by begin() and end(), to stream.
 *
 * \tparam T Value type.
 * \param os Output stream.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& ValueFormatter<T>::map(std::ostream& os) const noexcept {
    os << '{';
    // Print first object without comma
    auto it = m_val.begin();
    if (it != m_val.end()) {
        os << format_value(it->first) << ": " << format_value(it->second);
        ++it;
    }
    // Print the rest
    for (; it != m_val.end(); ++it) {
        os << ", " << format_value(it->first) << ": "
           << format_value(it->second);
    }
    os << '}';

    return os;
}

/** \brief Write a stack, defined by top(), to stream.
 *
 * \tparam T Value type.
 * \param os Output stream.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& ValueFormatter<T>::stack(std::ostream& os) const noexcept {
    // Only print first element, if available
    os << '{';
    if (m_val.size() == 1) {
        os << format_value(m_val.top());
    } else if (!m_val.empty()) {
        os << format_value(m_val.top()) << ", ...";
    }
    os << '}';

    return os;
}

/** \brief Write a queue, defined by front() and back(), to stream.
 *
 * \tparam T Value type.
 * \param os Output stream.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& ValueFormatter<T>::queue(std::ostream& os) const noexcept {
    // Only print first element, if available
    os << '{';
    if (m_val.size() == 1) {
        os << format_value(m_val.front());
    } else if (m_val.size() == 2) {
        os << format_value(m_val.front()) << ", " << format_value(m_val.back());
    } else if (!m_val.empty() != 0) {
        os << format_value(m_val.front()) << ", ..., "
           << format_value(m_val.back());
    }
    os << '}';

    return os;
}

/** \brief General value formatter.
 *
 * \tparam T Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<T>& f) noexcept {
    return os << f.m_val;
}

/** \brief Format bool.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<bool>& f) noexcept {
    return os << (f.m_val ? "true" : "false");
}

// 3.9.1 in C++ standard: Plain char, signed char, and unsigned
// char are three distinct types

/** \brief Format char.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<char>& f) noexcept {
    return os << '\'' << f.m_val << '\'';
}

/** \brief Format unsigned char.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<unsigned char>& f) noexcept {
    return os << '\'' << f.m_val << '\'';
}

/** \brief Format signed char.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<signed char>& f) noexcept {
    return os << '\'' << f.m_val << '\'';
}

/** \brief Format char*.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<char*>& f) noexcept {
    return os << '\"' << f.m_val << '\"';
}

/** \brief Format const char*.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<const char*>& f) noexcept {
    return os << '\"' << f.m_val << '\"';
}

/** \brief Format std::tm.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::tm>& f) noexcept {
    // Format as "YYYY-MM-DD HH:MM:SS", including null pointer
    char buf[20];
    if (std::strftime(buf, sizeof(buf), "%F %T", &f.m_val) == 0) {
        os << "std::tm format error";
    } else {
        os << buf;
    }
    return os;
}

/** \brief Format std::div_t.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::div_t>& f) noexcept {
    return os << "{quot = " << f.m_val.quot << ", rem = " << f.m_val.rem << '}';
}

/** \brief Format std::ldiv_t.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::ldiv_t>& f) noexcept {
    return os << "{quot = " << f.m_val.quot << ", rem = " << f.m_val.rem << '}';
}

/** \brief Format std::lldiv_t.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::lldiv_t>& f) noexcept {
    return os << "{quot = " << f.m_val.quot << ", rem = " << f.m_val.rem << '}';
}

/** \brief Format std::string.
 *
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::string>& f) noexcept {
    return os << '\"' << f.m_val << '\"';
}

/** \brief Format std::array.
 *
 * \tparam U Value type.
 * \tparam N Number of elements.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U, size_t N>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::array<U, N>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::vector.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::vector<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::deque.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::deque<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::forward_list.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::forward_list<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::list.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::list<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::set.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::set<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::map.
 *
 * \tparam U Key type.
 * \tparam V Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U, class V>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::map<U, V>>& f) noexcept {
    return f.map(os);
}

/** \brief Format std::multiset.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::multiset<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::multimap.
 *
 * \tparam U Key type.
 * \tparam V Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U, class V>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::multimap<U, V>>& f) noexcept {
    return f.map(os);
}

/** \brief Format std::unordered_set.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::unordered_set<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::unordered_map.
 *
 * \tparam U Key type.
 * \tparam V Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U, class V>
std::ostream& operator<<(std::ostream&              os,
    const ValueFormatter<std::unordered_map<U, V>>& f) noexcept {
    return f.map(os);
}

/** \brief Format std::unordered_multiset.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(std::ostream&                os,
    const ValueFormatter<std::unordered_multiset<U>>& f) noexcept {
    return f.sequence(os);
}

/** \brief Format std::unordered_multimap.
 *
 * \tparam U Key type.
 * \tparam V Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U, class V>
std::ostream& operator<<(std::ostream&                   os,
    const ValueFormatter<std::unordered_multimap<U, V>>& f) noexcept {
    return f.map(os);
}

/** \brief Format std::stack.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::stack<U>>& f) noexcept {
    return f.stack(os);
}

/** \brief Format std::queue.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(
    std::ostream& os, const ValueFormatter<std::queue<U>>& f) noexcept {
    return f.queue(os);
}

/** \brief Format std::priority_queue.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param f  ValueFormatter.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(std::ostream&            os,
    const ValueFormatter<std::priority_queue<U>>& f) noexcept {
    return f.stack(os);
}

/** \brief Write ANSI color start code to stream, if color is enabled.
 *
 * \param os Output stream.
 * \return Output stream.
 *
 */
std::ostream& color_start(std::ostream& os) noexcept {
    if (colorEnabled) {
        // Red
        os << "\033[0;31m";
    }
    return os;
}

/** \brief Write ANSI color end code to stream, if color is enabled.
 *
 * \param os Output stream.
 * \return Output stream.
 *
 */
std::ostream& color_end(std::ostream& os) noexcept {
    if (colorEnabled) {
        os << "\033[0m";
    }
    return os;
}

/** \brief Write type name to stream.
 *
 * \param os Output stream.
 * \return Output stream.
 *
 */
std::ostream& type_name(std::ostream& os) {
    if ((curPrefixes & prefix::TYPE_NAME) != prefix::NONE) {
        os << "type ";
    }
    return os;
}

/** \brief Array.
 *
 * \tparam T Value type.
 *
 */
template<class T>
class Array {
  public:
    /** \brief Construct.
     *
     * \param name      Name.
     * \param val       Values [\p len].
     * \param len       Number of values.
     * \param prefixFmt PrefixFormatter.
     *
     */
    explicit Array(const char* name, T& val, size_t len,
        const PrefixFormatter& prefixFmt) noexcept :
        m_name(name),
        m_val(val), m_len(len), m_prefixFmt(prefixFmt) {
    }

    template<class U>
    friend std::ostream& operator<<(
        std::ostream& os, const Array<U>& a) noexcept;

    /**
     * \return Name.
     */
    const char* get_name() const noexcept {
        return m_name;
    }

    /**
     * \return Values.
     */
    T& get_values() const noexcept {
        return m_val;
    }

    /**
     * \return Number of values.
     */
    size_t get_number_of_values() const noexcept {
        return m_len;
    }

    /**
     * \return PrefixFormatter.
     */
    const PrefixFormatter& get_prefix_formatter() const noexcept {
        return m_prefixFmt;
    }

  private:
    const char*            m_name;      /**< Name. */
    T&                     m_val;       /**< Values. */
    const size_t           m_len;       /**< Number of values. */
    const PrefixFormatter& m_prefixFmt; /**< PrefixFormatter. */
};

/** \brief Write Array to stream.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param a  Array.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(std::ostream& os, const Array<U>& a) noexcept {
    if (outputEnabled) {
        os << color_start << a.get_prefix_formatter() << type_name
           << a.get_name() << " = {";
        // Print first object without comma
        if (a.get_number_of_values() > 0) {
            os << format_value(a.get_values()[0]);
        }
        // Print the rest
        for (size_t i = 1; i < a.get_number_of_values(); ++i) {
            os << ", " << format_value(a.get_values()[i]);
        }
        os << '}' << color_end << GL_NEWLINE;
    }

    return os;
}

/** \brief Create array.
 *  This solves the error "cannot refer to class template 'Array' without a
 * template argument list".
 *
 * \tparam T Value type.
 * \param name      Name.
 * \param val       Values.
 * \param len       Number of values.
 * \param prefixFmt PrefixFormatter.
 * \return Array.
 *
 */
template<class T>
Array<T> make_array(
    const char* name, T& val, size_t len, const PrefixFormatter& prefixFmt) {
    return Array<T>(name, val, len, prefixFmt);
};

/** \brief Matrix.
 *
 * \tparam T Value type.
 *
 */
template<class T>
class Matrix {
  public:
    /** \brief Construct.
     *
     * \param name      Name.
     * \param val       Values [\p cols x \p rows].
     * \param cols      Number of columns.
     * \param rows      Number of rows.
     * \param prefixFmt PrefixFormatter.
     *
     */
    explicit Matrix(const char* name, T& val, size_t cols, size_t rows,
        const PrefixFormatter& prefixFmt) noexcept :
        m_name(name),
        m_val(val), m_cols(cols), m_rows(rows), m_prefixFmt(prefixFmt) {
    }

    template<class U>
    friend std::ostream& operator<<(
        std::ostream& os, const Matrix<U>& a) noexcept;

    /**
     * \return Name.
     */
    const char* get_name() const noexcept {
        return m_name;
    }

    /**
     * \return Values.
     */
    T& get_values() const noexcept {
        return m_val;
    }

    /**
     * \return Number of columns.
     */
    size_t get_number_of_columns() const noexcept {
        return m_cols;
    }

    /**
     * \return Number of rows.
     */
    size_t get_number_of_rows() const noexcept {
        return m_rows;
    }

    /**
     * \return PrefixFormatter.
     */
    const PrefixFormatter& get_prefix_formatter() const noexcept {
        return m_prefixFmt;
    }

  private:
    const char*            m_name;      /**< Name. */
    T&                     m_val;       /**< Values. */
    const size_t           m_cols;      /**< Number of columns. */
    const size_t           m_rows;      /**< Number of rows. */
    const PrefixFormatter& m_prefixFmt; /**< PrefixFormatter. */
};

/** \brief Write Matrix to stream.
 *
 * \tparam U Value type.
 * \param os Output stream.
 * \param m  Matrix.
 * \return Output stream.
 *
 */
template<class U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& m) noexcept {
    if (outputEnabled) {
        os << color_start << m.get_prefix_formatter() << type_name
           << m.get_name() << ": ";
        if (m.get_number_of_columns() <= 0 || m.get_number_of_rows() <= 0) {
            os << "{}";
        } else {
            os << "[0,0] = " << format_value(m.get_values()[0][0]);
            for (size_t j = 1; j < m.get_number_of_columns(); ++j) {
                os << ", "
                   << "[0," << j
                   << "] = " << format_value(m.get_values()[0][j]);
            }
            for (size_t i = 1; i < m.get_number_of_rows(); ++i) {
                for (size_t j = 0; j < m.get_number_of_columns(); ++j) {
                    os << ", "
                       << "[" << i << ',' << j
                       << "] = " << format_value(m.get_values()[i][j]);
                }
            }
        }
        os << color_end << GL_NEWLINE;
    }

    return os;
}

/** \brief Create matrix.
 *  This solves the error "cannot refer to class template 'Matrix' without a
 * template argument list".
 *
 * \tparam T Value type.
 * \param name      Name.
 * \param val       Values.
 * \param cols      Number of columns.
 * \param rows      Number of rows.
 * \param prefixFmt PrefixFormatter.
 *
 * \return New matrix.
 *
 */
template<class T>
Matrix<T> make_matrix(const char* name, T& val, size_t cols, size_t rows,
    const PrefixFormatter& prefixFmt) {
    return Matrix<T>(name, val, cols, rows, prefixFmt);
};

} // namespace internal

#endif // DOXYGEN_HIDDEN

/**
 *
 * \return Bitwise \c or of prefix settings.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
prefix get_prefixes() noexcept {
    return internal::curPrefixes;
}

/** \brief Set prefixes of logging output.
 *
 * Alters the output of \ref l(), \ref l_arr(), and \ref l_mat().
 *
 * \param p Bitwise \c or of prefix settings.
 *
 * Used as:
 * \code
 * gl::set_prefixes(gl::prefix::FILE | gl::prefix::LINE);
 * \endcode
 * to also log current file and line number.
 *
 * \note Defaults to prefix::FILE | prefix::LINE.
 *
 * \sa prefix \sa get_prefixes()
 *
 */
void set_prefixes(prefix p) noexcept {
    internal::curPrefixes = p;
}

/**
 *
 * \return \c true if output is enabled.
 *
 * \sa set_output_enabled()
 *
 */
bool is_output_enabled() noexcept {
    return internal::outputEnabled;
}

/** \brief Enable or disable output.
 *
 * \param e \c true if output shall be enabled.
 *
 * \note Defaults to enabled.
 *
 * \sa is_output_enabled()
 *
 */
void set_output_enabled(bool e) noexcept {
    internal::outputEnabled = e;
}

/** \brief Enable or disable ANSI color output.
 *
 * \param e \c true if output shall be colored.
 *
 * \note Defaults to disabled.
 *
 * \sa is_color_enabled()
 *
 */
void set_color_enabled(bool e) noexcept {
    internal::colorEnabled = e;
}

/**
 *
 * \return \c true if ANSI color output is enabled.
 *
 * \sa set_color_enabled()
 *
 */
bool is_color_enabled() noexcept {
    return internal::colorEnabled;
}

#ifndef DOXYGEN_HIDDEN

/** \brief Helper function for l(). Dispatch to the right macro depending on
 * number of arguments.
 *
 */
#define GL_INTERNAL_L_DISPATCH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, \
    _12, _13, _14, _15, _16, NAME, ...)                                      \
    NAME

/** \brief Output variable name and value. */
#define GL_INTERNAL_LX(v)                    \
    gl::internal::type_name << (#v) << " = " \
                            << ::gl::internal::format_value((v))

#define GL_INTERNAL_L1(v1) GL_INTERNAL_LX(v1)

#define GL_INTERNAL_L2(v1, v2) GL_INTERNAL_L1(v1) << ", " << GL_INTERNAL_LX(v2)

#define GL_INTERNAL_L3(v1, v2, v3) \
    GL_INTERNAL_L2(v1, v2) << ", " << GL_INTERNAL_LX(v3)

#define GL_INTERNAL_L4(v1, v2, v3, v4) \
    GL_INTERNAL_L3(v1, v2, v3) << ", " << GL_INTERNAL_LX(v4)

#define GL_INTERNAL_L5(v1, v2, v3, v4, v5) \
    GL_INTERNAL_L4(v1, v2, v3, v4) << ", " << GL_INTERNAL_LX(v5)

#define GL_INTERNAL_L6(v1, v2, v3, v4, v5, v6) \
    GL_INTERNAL_L5(v1, v2, v3, v4, v5) << ", " << GL_INTERNAL_LX(v6)

#define GL_INTERNAL_L7(v1, v2, v3, v4, v5, v6, v7) \
    GL_INTERNAL_L6(v1, v2, v3, v4, v5, v6) << ", " << GL_INTERNAL_LX(v7)

#define GL_INTERNAL_L8(v1, v2, v3, v4, v5, v6, v7, v8) \
    GL_INTERNAL_L7(v1, v2, v3, v4, v5, v6, v7) << ", " << GL_INTERNAL_LX(v8)

#define GL_INTERNAL_L9(v1, v2, v3, v4, v5, v6, v7, v8, v9) \
    GL_INTERNAL_L8(v1, v2, v3, v4, v5, v6, v7, v8) << ", " << GL_INTERNAL_LX(v9)

#define GL_INTERNAL_L10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
    GL_INTERNAL_L9(v1, v2, v3, v4, v5, v6, v7, v8, v9)           \
        << ", " << GL_INTERNAL_LX(v10)

#define GL_INTERNAL_L11(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
    GL_INTERNAL_L10(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)          \
        << ", " << GL_INTERNAL_LX(v11)

#define GL_INTERNAL_L12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
    GL_INTERNAL_L11(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)          \
        << ", " << GL_INTERNAL_LX(v12)

#define GL_INTERNAL_L13(                                               \
    v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)            \
    GL_INTERNAL_L12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
        << ", " << GL_INTERNAL_LX(v13)

#define GL_INTERNAL_L14(                                                    \
    v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)            \
    GL_INTERNAL_L13(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
        << ", " << GL_INTERNAL_LX(v14)

#define GL_INTERNAL_L15(                                              \
    v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) \
    GL_INTERNAL_L14(                                                  \
        v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)  \
        << ", " << GL_INTERNAL_LX(v15)

#define GL_INTERNAL_L16(                                                   \
    v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) \
    GL_INTERNAL_L15(                                                       \
        v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)  \
        << ", " << GL_INTERNAL_LX(v16)

#endif // DOXYGEN_HIDDEN

} // namespace gl

#endif // INCLUDE_GOINGLOGGING_H_
