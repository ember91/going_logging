#include "goinglogging/goinglogging.h"
#include "test/test.h"
#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

/** Test outputting a class by overriding the << operator */
class TestOp {
  public:
    friend std::ostream& operator<<(std::ostream& out, const TestOp& t);
};

/** Output class information
 *
 * \param out Output stream
 * \param t   Object to output
 * \return    Stream
 *
 */
std::ostream& operator<<(std::ostream& out, const TestOp& t) {
    (void)(t);
    out << "TestOp";
    return out;
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    int         a = 1;
    const char* b = "hej";
    uint32_t    c = 7;
    int*        d = nullptr;
    bool        e = true;
    std::string f("asdsa");
    int*        g = &a;
    TestOp      h;
    int&        i = a;
    char        j = 'j';

    l(a);
    l(a, b);
    l(a, b, c);
    l(a, b, c, d);
    l(a, b, c, d, e);
    l(a, b, c, d, e, f);
    l(a, b, c, d, e, f, g);
    l(a, b, c, d, e, f, g, h);
    l(a, b, c, d, e, f, g, h, i);
    l(a, b, c, d, e, f, g, h, i, j);
    l(a, b, c, d, e, f, g, h, i, j, *g);
    l(a, b, c, d, e, f, g, h, i, j, *g, a);
    l(a, b, c, d, e, f, g, h, i, j, *g, a, b);

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

    return t.compare_output(true);
}
