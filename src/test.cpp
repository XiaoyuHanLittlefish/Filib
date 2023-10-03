/*** 
 * @Author: 469714875@qq.com
 * @Date: 2023-10-02 14:58:46
 * @LastEditors: 469714875@qq.com
 * @LastEditTime: 2023-10-03 16:39:28
 * @FilePath: \FiLib\src\test.cpp
 * @Description: 
 * @
 * @Copyright (c) 2023 by littlefishOvO, All Rights Reserved. 
 */

#include <iostream>
#include <fstream>
#include <set>
#include <stdlib.h>
#include <time.h>

#include "magnum/frbtree.h"

void test() {
    frbtree::Rbtree<int> rbtree;
    std::set<int>        s;
    constexpr int        len = 10000000;
    constexpr int        max = 1000000000;
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
            return;
        }
    }
    // std::cout << "start erase" << std::endl;
    // for (int i = 0; i < len; i++) {
    //     int x = rand() % max;
    //     s.erase(x);
    //     rbtree.erase(x);
    // }
    // std::cout << "start find 2" << std::endl;
    // for (int i = 0; i < len; i++) {
    //     int x               = rand() % max;
    //     int set_find_res    = (s.find(x) != s.end());
    //     int rbtree_find_res = rbtree.find(x);
    //     if (set_find_res != rbtree_find_res) {
    //         std::cout << "failed to find " << x << " in 2" << std::endl;
    //         return;
    //     }
    // }
    std::cout << "success" << std::endl;
}

int main() {
    test();
}