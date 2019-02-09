#include "goinglogging.h"
#include "test/test.h"
#include <array>
#include <atomic>
#include <bitset>
#include <complex>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <valarray>
#include <vector>

/**
 * \file
 * Test logging output of types in the C++ standard library.
 */

using namespace gl::test;

/**
 * \brief Log std:.bitset.
 */
void log_bitset() {
    std::bitset<8> bs(0xaa);

    l(bs);
}

/**
 * \brief Log pair types.
 */
void log_pairs() {
    std::pair<int, float>                         pr(0, 5.0f);
    std::tuple<int, float>                        tp(0, 5.0f);
    std::pair<std::pair<float, const char*>, int> adv = {{3.4, "p"}, 3};

    l(pr);
    l(tp);
    l(adv);
}

/**
 * \brief Log pointer types.
 */
void log_pointers() {
    std::unique_ptr<int> up(new int(3));
    std::shared_ptr<int> sp(new int(3));
    std::weak_ptr<int>   wp = sp;

    l(up);
    l(sp);
    l(wp);
}

/**
 * \brief Log complex types.
 */
void log_complex() {
    std::complex<float>       cf(1.0f, 2.0f);
    std::complex<double>      cd(1.0, -2.0);
    std::complex<long double> cld(-1.0L, -2.0L);

    l(cf);
    l(cd);
    l(cld);
}

/**
 * \brief Log std::valarray.
 *
 */
void log_valarray() {
    std::valarray<int> varr = {0, 1, 2, 3};

    l(varr);
}

/**
 * \brief log std::ratio.
 *
 */
void log_ratio() {
    std::ratio<2, 10> rat;

    l(rat);
}

/**
 * \brief Log stringstream types.
 *
 */
void log_stringstreams() {
    std::stringbuf      sb("sb");
    std::wstringbuf     wsb(L"wsb");
    std::ostringstream  oss("oss");
    std::wostringstream woss(L"woss");
    std::stringstream   ss("ss");
    std::wstringstream  wss(L"wss");

    l(sb);
    l(wsb);
    l(oss);
    l(woss);
    l(ss);
    l(wss);
}

/**
 * \brief Log std::locale.
 *
 */
void log_locale() {
    std::locale loc("");

    l(loc);
}

/**
 * \brief Log atomic types.
 */
void log_atomic() {
    std::atomic<int> ati(4);

    l(ati);
}

/**
 * \brief Log string types.
 */
void log_strings() {
    std::string    str   = "str";
    std::u16string str16 = u"str16";
    std::u32string str32 = U"str32";
    std::wstring   strw  = L"strw";

    l(str);
    l(str16);
    l(str32);
    l(strw);
}

/**
 * \brief Log container types.
 */
void log_containers() {
    std::array<int, 3>      arr  = {0, 1, 2};
    std::deque<int>         deq  = {0, 1, 2};
    std::vector<int>        vec  = {0, 1, 2};
    std::forward_list<int>  fl   = {0, 1, 2};
    std::list<int>          lst  = {0, 1, 2};
    std::set<int>           set  = {0, 1, 2};
    std::map<int, int>      map  = {{0, 1}, {2, 3}, {4, 5}};
    std::multiset<int>      mset = {0, 1, 2};
    std::multimap<int, int> mmap = {{0, 1}, {2, 3}, {4, 5}};
    std::stack<int>         st;
    st.push(2);
    st.push(1);
    st.push(0);
    std::queue<int> que;
    que.push(0);
    que.push(1);
    que.push(2);
    std::priority_queue<int> pque;
    pque.push(0);
    pque.push(1);
    pque.push(2);
    std::map<const char*, std::vector<std::pair<float, std::string>>> adv = {
        {"a", {{1.0, "1"}, {2.0, "2"}, {3.0, "3"}}},
        {"b", {{1.0, "1"}, {2.0, "2"}, {3.0, "3"}}}};

    l(arr);
    l(deq);
    l(vec);
    l(fl);
    l(lst);
    l(set);
    l(map);
    l(mset);
    l(mmap);
    l(st);
    l(que);
    l(pque);
    l(adv);
}

/**
 * \brief Test entry point.
 *
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \return EXIT_SUCCESS if success.
 */
int main(int argc, const char** argv) {
    // Check number of arguments
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    // Disable prefixes for easier output comparison.
    gl::set_prefixes(gl::prefix::NONE);

    Test t;
    t.setup(__FILE__);

    // Log
    log_bitset();
    log_pairs();
    log_pointers();
    log_complex();
    log_valarray();
    log_ratio();
    log_stringstreams();
    log_locale();
    log_atomic();
    log_strings();
    log_containers();

    // Compare output
    return t.compare_output(Test::ComparisonMode::REGEX);
}
