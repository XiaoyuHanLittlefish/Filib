#include <iostream>

#include "magnum/frbtree.h"

int main() {
    frbtree::Rbtree<int> rbtree;
    std::cout << rbtree.find(1) << std::endl;
    std::cout << rbtree.find(2) << std::endl;
    std::cout << rbtree.find(3) << std::endl;
    std::cout << rbtree.find(4) << std::endl;
    std::cout << rbtree.find(5) << std::endl;
    rbtree.push(4);
    rbtree.push(2);
    rbtree.push(5);
    rbtree.push(1);
    std::cout << rbtree.find(1) << std::endl;
    std::cout << rbtree.find(2) << std::endl;
    std::cout << rbtree.find(3) << std::endl;
    std::cout << rbtree.find(4) << std::endl;
    std::cout << rbtree.find(5) << std::endl;
}