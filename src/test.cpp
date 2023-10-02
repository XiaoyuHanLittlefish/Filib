/*
 * @Author: 469714875@qq.com 469714875@qq.com
 * @Date: 2023-09-24 12:12:32
 * @LastEditors: 469714875@qq.com 469714875@qq.com
 * @LastEditTime: 2023-10-02 14:43:32
 * @FilePath: \FiLib\src\test.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <fstream>
#include <set>
#include <stdlib.h>
#include <time.h>

#include "magnum/frbtree.h"

int main() {
    frbtree::Rbtree<int> rbtree;
    std::set<int>        s;
    constexpr int        len = 1000000;
    constexpr int        max = 1000000;
    srand((unsigned)time(NULL));
    std::cout << "start insert" << std::endl;
    for (int i = 0; i < len; i++) {
        int x = rand() % max;
        s.insert(x);
        rbtree.push(x);
    }
    std::cout << "start find 1" << std::endl;
    for (int i = 0; i < len; i++) {
        int x               = rand() % max;
        int set_find_res    = (s.find(x) != s.end());
        int rbtree_find_res = rbtree.find(x);
        if (set_find_res != rbtree_find_res) {
            std::cout << "failed to find " << x << " in 1" << std::endl;
            return 0;
        }
    }
    std::cout << "start erase" << std::endl;
    for (int i = 0; i < len; i++) {
        int x = rand() % max;
        s.erase(x);
        rbtree.erase(x);
    }
    std::cout << "start find 2" << std::endl;
    for (int i = 0; i < len; i++) {
        int x               = rand() % max;
        int set_find_res    = (s.find(x) != s.end());
        int rbtree_find_res = rbtree.find(x);
        if (set_find_res != rbtree_find_res) {
            std::cout << "failed to find " << x << " in 2" << std::endl;
            return 0;
        }
    }
    std::cout << "success" << std::endl;
}