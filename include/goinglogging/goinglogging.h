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

#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

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
 */
#define GL_NEWLINE '\n'
#endif // GL_NEWLINE

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
    NONE     = 0,      /**< No prefix. */
    FILE     = 1 << 0, /**< File name. For example 'main.cpp'. */
    LINE     = 1 << 1, /**< Line number in file. For example 'Line: 16'. */
    FUNCTION = 1 << 2, /**< Function name. For example 'calculate()'. */
    TIME = 1 << 3, /**< Current local time as hour:minute:second.millisecond.
                    *  For example '10:02:13.057'. */
    THREAD    = 1 << 4, /**< ID of current thread. For example 'TID: 12'. */
    TYPE_NAME = 1 << 5  /**< Name of type. For example 'int'. */
};

namespace internal {

template<class T>
void array(const char* var_name, const char* file_path, long file_line,
    const char* func, const T var_val, size_t len) noexcept;
template<class T>
void matrix(const char* var_name, const char* file_path, long file_line,
    const char* func, const T var_val, size_t cols, size_t rows) noexcept;

} // namespace internal

prefix get_prefixes() noexcept;
void   set_prefixes(prefix p) noexcept;
bool   is_output_enabled() noexcept;
void   set_output_enabled(bool e) noexcept;
bool   is_color_enabled() noexcept;
void   set_color_enabled(bool e) noexcept;

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

/** \brief Functionality in this namespace is for internal use */
namespace internal {

/** Current prefixes */
static prefix curPrefixes = prefix::FILE | prefix::LINE;
/** \c true if output is enabled */
static bool outputEnabled = true;
/** \c true if color is enabled */
static bool colorEnabled = false;

/** \brief Generate prefix to stream.
 *
 * \note For internal use.
 *
 */
class Prefixer {
  public:
    /** \brief Create.
     *
     * \param file_path File path including name.
     * \param file_line Line number in file.
     * \param func      Function name.
     *
     */
    Prefixer(const char* file_path, long file_line, const char* func) noexcept :
        m_file_path(file_path), m_file_line(file_line), m_func(func) {
    }

    friend std::ostream& operator<<(
        std::ostream& os, const Prefixer& p) noexcept;

    /** \brief Get current file path.
     *
     * \return Current file path.
     *
     */
    const char* get_file_path() const noexcept {
        return m_file_path;
    }

    /** \brief Get current line number in file.
     *
     * \return Current line number in file.
     *
     */
    long get_file_line_number() const noexcept {
        return m_file_line;
    }

    /** \brief Get current function name.
     *
     * \return Current function name.
     *
     */
    const char* get_function_name() const noexcept {
        return m_func;
    }

  private:
    const char* m_file_path; /**< File path including name */
    long        m_file_line; /**< Line number in file */
    const char* m_func;      /**< Function name */
};

/** \brief Write to stream.
 *
 * \param os Output stream.
 * \param p  Object to output.
 * \return Output stream.
 *
 */
std::ostream& operator<<(std::ostream& os, const Prefixer& p) noexcept {
    /** Number of prefixes written */
    uint32_t cnt = 0;

    // FILE
    if ((curPrefixes & prefix::FILE) != prefix::NONE) {
        // Path separator
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
        // No need to check for str_len = 0 here, since that will stop the for
        // loop anyway
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
        auto        now        = std::chrono::system_clock::now();
        std::time_t sinceEpoch = std::chrono::system_clock::to_time_t(now);
        auto        ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

        std::tm* local = std::localtime(&sinceEpoch);
        if (local != nullptr) {
            if (cnt != 0) {
                os << ", ";
            }
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

/** \brief Stringifies a variable to stream.
 *
 * \note For internal use.
 * \tparam T Variable type.
 *
 */
template<class T>
class Stringifier {
  public:
    /** \brief Create object with variable.
     *
     * \param t Variable.
     *
     */
    explicit Stringifier(T& t) noexcept : m_t(t) {
    }

    /** \brief Write to stream.
     *
     * \tparam U Variable type.
     * \param os Output stream.
     * \param s  Object to output.
     * \return Output stream.
     *
     */
    template<class U>
    friend std::ostream& operator<<(
        std::ostream& os, const Stringifier<U>& s) noexcept;

    /** \brief Get variable.
     *
     * \return Variable
     *
     */
    T& get_t() const noexcept {
        return m_t;
    }

  private:
    T& m_t; /**< Variable to stringify. */
};

/** \brief General Stringifier output function.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param os Output stream.
 * \param s  Stringifier.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const Stringifier<T>& s) noexcept {
    return os << s.get_t();
}

/** \brief Stringifier char specialized output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param s  Stringifier.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const Stringifier<char>& s) noexcept {
    return os << '\'' << s.get_t() << '\'';
}

/** \brief Stringifier C string specialized output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param s  Stringifier.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const Stringifier<char*>& s) noexcept {
    return os << '\"' << s.get_t() << '\"';
}

/** \brief Stringifier C string specialized output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param s  Stringifier.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const Stringifier<const char*>& s) noexcept {
    return os << '\"' << s.get_t() << '\"';
}

/** \brief Stringifier C++ string specialized output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param s  Stringifier.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const Stringifier<std::string>& s) noexcept {
    return os << '\"' << s.get_t() << '\"';
}

/** \brief Create Stringifier from scratch.
 *  This solves the error "cannot refer to class template 'Stringifier' without
 * a template argument list".
 *
 * \note For internal use.
 * \tparam T variable type.
 * \param t Variable.
 * \return Created Stringifier.
 */
template<class T>
Stringifier<T> stringify(T& t) {
    return Stringifier<T>(t);
};

/** \brief Write name of a type to stream.
 *
 * \note For internal use.
 * \tparam T Variable type.
 *
 */
template<class T>
class TypeNamer {
  public:
    /** \brief Create object with variable.
     *
     * \param t        Variable.
     * \param is_first \c true if first TyepNamer.
     *
     */
    explicit TypeNamer(T& t, bool is_first) noexcept :
        m_t(t), m_is_first(is_first) {
    }

    /** \brief Write to stream.
     *
     * \tparam U Variable type.
     * \param os Output stream.
     * \param t  Object to output.
     * \return Output stream.
     *
     */
    template<class U>
    friend std::ostream& operator<<(
        std::ostream& os, const TypeNamer<U>& t) noexcept;

    /** \brief Helper function for insertion stream operator. Outputs a string
     * if enabled.
     *
     * \param os Output stream.
     * \param s  Output string.
     * \return Output stream.
     *
     */
    std::ostream& out(std::ostream& os, const char* s) const noexcept {
        if ((curPrefixes & prefix::TYPE_NAME) != prefix::NONE) {
            os << s;
            if (m_is_first) {
                os << ' ';
            }
        }
        return os;
    }

    /** \brief Get variable.
     *
     * \return Variable.
     *
     */
    T& get_t() const noexcept {
        return m_t;
    }

    /** \brief Check if this is the first TypeNamer.
     *
     * \return \c true if first TypeNamer.
     *
     */
    bool is_first() const noexcept {
        return m_is_first;
    }

  private:
    T&   m_t;        /**< Variable to get type name of. */
    bool m_is_first; /**< \c true if first TypeNamer */
};

/** \brief General TypeNamer output function.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const TypeNamer<T>& t) noexcept {
    return t.out(os, typeid(t.get_t()).name());
}

/** \brief General TypeNamer output function.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param os    Output stream.
 * \param t     TypeNamer.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<const T>& t) noexcept {
    return t.out(os, typeid(t.get_t()).name());
}

/** \brief Pointer TypeNamer output function.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param os    Output stream.
 * \param t     TypeNamer.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const TypeNamer<T*>& t) noexcept {
    if ((curPrefixes & prefix::TYPE_NAME) != prefix::NONE) {
        os << TypeNamer<T>(*t.get_t(), false) << '*';
        if (t.is_first()) {
            os << ' ';
        }
    }
    return os;
}

/** \brief Const pointer TypeNamer output function.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param os    Output stream.
 * \param t     TypeNamer.
 * \return Output stream.
 *
 */
template<class T>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<const T*>& t) noexcept {
    if ((curPrefixes & prefix::TYPE_NAME) != prefix::NONE) {
        T* non_const = const_cast<T*>(t.get_t());
        os << "const " << TypeNamer<T*>(non_const, false);
        if (t.is_first()) {
            os << ' ';
        }
    }
    return os;
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<uint8_t>& t) noexcept {
    return t.out(os, "uint8_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<uint16_t>& t) noexcept {
    return t.out(os, "uint16_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<uint32_t>& t) noexcept {
    return t.out(os, "uint32_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<uint64_t>& t) noexcept {
    return t.out(os, "uint64_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<int8_t>& t) noexcept {
    return t.out(os, "int8_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<int16_t>& t) noexcept {
    return t.out(os, "int16_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<int32_t>& t) noexcept {
    return t.out(os, "int32_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<int64_t>& t) noexcept {
    return t.out(os, "int64_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(std::ostream& os, const TypeNamer<float>& t) noexcept {
    return t.out(os, "float");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<double>& t) noexcept {
    return t.out(os, "double");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(std::ostream& os, const TypeNamer<char>& t) noexcept {
    return t.out(os, "char");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<char16_t>& t) noexcept {
    return t.out(os, "char16_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<char32_t>& t) noexcept {
    return t.out(os, "char32_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<wchar_t>& t) noexcept {
    return t.out(os, "wchar_t");
}

/** \brief Specialized TypeNamer output function.
 *
 * \note For internal use.
 * \param os Output stream.
 * \param t  TypeNamer.
 * \return Output stream.
 *
 */
template<>
std::ostream& operator<<(
    std::ostream& os, const TypeNamer<std::string>& t) noexcept {
    return t.out(os, "std::string");
}

/** \brief Create TypeNamer from scratch.
 *  This solves the error "cannot refer to class template 'TypeNamer' without a
 * template argument list".
 *
 * \note For internal use.
 * \tparam T variable type.
 * \param t        Variable.
 * \param is_first \c true if first TypeNamer.
 * \return Created TypeNamer.
 */
template<class T>
TypeNamer<T> type_name(T& t, bool is_first) {
    return TypeNamer<T>(t, is_first);
};

/** \brief Output ANSI color start code, if color is enabled.
 *
 * \note For internal use.
 * \param os Output stream.
 * \return Output stream.
 *
 */
std::ostream& color_start(std::ostream& os) noexcept {
    if (colorEnabled) {
        os << "\033[0;31m";
    }
    return os;
}

/** \brief Output ANSI color end code, if color is enabled.
 *
 * \note For internal use.
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

/** \brief Log array.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param var_name  Variable name.
 * \param file_path Current File path including name.
 * \param file_line Current line number in file.
 * \param func      Current function name.
 * \param var_val   Variable values.
 * \param len       Number of elements.
 *
 */
template<class T>
void array(const char* var_name, const char* file_path, long file_line,
    const char* func, const T var_val, size_t len) noexcept {
    if (outputEnabled) {
        std::cout << color_start << Prefixer(file_path, file_line, func)
                  << type_name(var_val, true) << var_name << " = {";
        // Print first object without comma
        if (len > 0)
            std::cout << stringify(var_val[0]);
        // Print the rest
        for (size_t i = 1; i < len; ++i) {
            std::cout << ", " << stringify(var_val[i]);
        }
        std::cout << '}' << color_end << GL_NEWLINE;
    }
}

/** \brief Log matrix with dimensions \p c * \p r.
 *
 * \note For internal use.
 * \tparam T Variable type.
 * \param var_name  Variable name.
 * \param file_path Current file path including name.
 * \param file_line Current line number in file.
 * \param func      Current function name.
 * \param var_val   Variable values.
 * \param cols      Number of columns.
 * \param rows      Number of rows.
 *
 */
template<class T>
void matrix(const char* var_name, const char* file_path, long file_line,
    const char* func, const T var_val, size_t cols, size_t rows) noexcept {
    if (outputEnabled) {
        std::cout << color_start << Prefixer(file_path, file_line, func)
                  << type_name(var_val, true) << var_name << ": ";
        if (cols <= 0 || rows <= 0) {
            std::cout << "{}";
        } else {
            std::cout << "[0,0] = " << stringify(var_val[0][0]);
            for (size_t j = 1; j < cols; ++j) {
                std::cout << ", "
                          << "[0," << j << "] = " << stringify(var_val[0][j]);
            }
            for (size_t i = 1; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    std::cout << ", "
                              << "[" << i << ',' << j
                              << "] = " << stringify(var_val[i][j]);
                }
            }
        }
        std::cout << color_end << GL_NEWLINE;
    }
}

} // namespace internal

#endif // DOXYGEN_HIDDEN

/** \brief Get prefixes of logging output.
 *
 * \return Prefix settings.
 *
 * \sa prefix \sa set_prefixes()
 *
 */
prefix get_prefixes() noexcept {
    return internal::curPrefixes;
}

/** \brief Set prefixes of logging output.
 *
 * Alters the output of \sa l(), \sa l_arr(), and \sa l_mat().
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

/** \brief Check if output is enabled.
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

/** \brief Check if ANSI color output is enabled.
 *
 * \return \c true if ANSI color output is enabled.
 *
 * \sa set_color_enabled()
 *
 */
bool is_color_enabled() noexcept {
    return internal::colorEnabled;
}

/** \brief Log variables to stdout.
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
 * i = 1, s = s
 * \endcode
 *
 * \note Supports up to 16 variables on the same line.
 * \note Uses prefix information set with \ref set_prefixes().
 *
 * \sa l_arr() \sa l_mat() \sa set_prefixes().
 *
 */
#define l(...)                                                                 \
    do {                                                                       \
        if (gl::internal::outputEnabled) {                                     \
            std::cout << gl::internal::color_start                             \
                      << gl::internal::Prefixer(__FILE__, __LINE__, __func__)  \
                      << GL_INTERNAL_GET_MACRO(__VA_ARGS__, GL_INTERNAL_L16,   \
                             GL_INTERNAL_L15, GL_INTERNAL_L14,                 \
                             GL_INTERNAL_L13, GL_INTERNAL_L12,                 \
                             GL_INTERNAL_L11, GL_INTERNAL_L10, GL_INTERNAL_L9, \
                             GL_INTERNAL_L8, GL_INTERNAL_L7, GL_INTERNAL_L6,   \
                             GL_INTERNAL_L5, GL_INTERNAL_L4, GL_INTERNAL_L3,   \
                             GL_INTERNAL_L2, GL_INTERNAL_L1, )(__VA_ARGS__)    \
                      << gl::internal::color_end << (GL_NEWLINE);              \
        }                                                                      \
    } while (false)

#ifndef DOXYGEN_HIDDEN

#define GL_INTERNAL_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, \
    _12, _13, _14, _15, _16, NAME, ...)                                     \
    NAME

#define GL_INTERNAL_LX(v)              \
    gl::internal::type_name((v), true) \
        << (#v) << " = " << gl::internal::stringify((v))

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

/** \brief Log array to stdout.
 *
 * \param v   Array to print.
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
 * a: {0, 1, 2}
 * \endcode
 *
 * \note Uses prefix information set with \ref set_prefixes().
 *
 * \warning Behaviour is undefined if \p n is larger than the allocated array.
 *
 * \sa l() \sa l_mat() \sa set_prefixes()
 *
 */
#define l_arr(v, len)                                                        \
    do {                                                                     \
        gl::internal::array((#v), __FILE__, __LINE__, __func__, (v), (len)); \
    } while (false)

/** \brief Log matrix to stdout.
 *
 * \param m    Matrix to print.
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
 *
 * \warning Behaviour is undefined if \p c or \p r is larger than the
 *          allocated matrix.
 *
 * \sa l() \sa l_arr() \sa set_prefixes()
 *
 */
#define l_mat(m, cols, rows)                                          \
    do {                                                              \
        gl::internal::matrix(                                         \
            (#m), __FILE__, __LINE__, __func__, (m), (cols), (rows)); \
    } while (false)

} // namespace gl

#endif // INCLUDE_GOINGLOGGING_H_
