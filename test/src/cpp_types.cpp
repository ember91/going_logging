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

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cout << "Usage: " << *argv << std::endl;
        return EXIT_SUCCESS;
    }

    gl::set_prefixes(gl::prefix::NONE);

    test t;
    t.setup(__FILE__);

    std::string             str  = "std::string";
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

    l(str);
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

    return t.compare_output(false);
}
