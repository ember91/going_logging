#include "goinglogging/goinglogging.h"
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

    std::bitset<8>         bs(0xaa);
    std::pair<int, float>  pr(0, 5.0f);
    std::tuple<int, float> tp(0, 5.0f);
    std::unique_ptr<int>   up(new int(3));
    std::shared_ptr<int>   sp(new int(3));
    std::weak_ptr<int>     wp = sp;

    std::complex<float>       cf(1.0f, 2.0f);
    std::complex<double>      cd(1.0, -2.0);
    std::complex<long double> cld(-1.0L, -2.0L);

    std::valarray<int> varr = {0, 1, 2, 3};

    std::ratio<2, 10> rat;

    std::stringbuf      sb("sb");
    std::wstringbuf     wsb(L"wsb");
    std::ostringstream  oss("oss");
    std::wostringstream woss(L"woss");
    std::stringstream   ss("ss");
    std::wstringstream  wss(L"wss");

    std::locale loc("");

    std::atomic<int> ati(4);

    std::string             str   = "str";
    std::u16string          str16 = u"str16";
    std::u32string          str32 = U"str32";
    std::wstring            strw  = L"strw";
    std::array<int, 3>      arr   = {0, 1, 2};
    std::deque<int>         deq   = {0, 1, 2};
    std::vector<int>        vec   = {0, 1, 2};
    std::forward_list<int>  fl    = {0, 1, 2};
    std::list<int>          lst   = {0, 1, 2};
    std::set<int>           set   = {0, 1, 2};
    std::map<int, int>      map   = {{0, 1}, {2, 3}, {4, 5}};
    std::multiset<int>      mset  = {0, 1, 2};
    std::multimap<int, int> mmap  = {{0, 1}, {2, 3}, {4, 5}};
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

    l(bs);
    l(pr);
    l(tp);
    l(up);
    l(sp);
    l(wp);
    l(cf);
    l(cd);
    l(cld);
    l(varr);
    l(rat);

    l(sb);
    l(wsb);
    l(oss);
    l(woss);
    l(ss);
    l(wss);

    l(loc);
    l(ati);
    l(str);
    l(str16);
    l(str32);
    l(strw);
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

    // Compare output
    return t.compare_output(Test::ComparisonMode::REGEX);
}
