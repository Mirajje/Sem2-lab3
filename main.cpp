#include <iostream>

#include "TernaryTree.hpp"
#include "TernaryTreeTests.h"

int main()
{
    TernaryTreeTests::allTestsTogether();
    TernaryTree<int> tree;
    tree.addElem("L", 15);
    tree.addElem("R", 42);
    tree.addElem("RR", 42);
    tree.addElem("RRR", 42);
    tree.addElem("M", 50);

    TernaryTree<int> tree2;
    tree2.addElem("M", 4);

    std::cout << tree.convertToString() << std::endl;
}
