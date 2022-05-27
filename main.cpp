#include <iostream>

#include "TernaryTree.hpp"
#include "TernaryTreeTests.h"

int main()
{
    TernaryTreeTests::allTestsTogether();
    TernaryTree<int> tree;
    //tree.addElem("", 15);
    //tree.addElem("R", 42);
    /*tree.addElem("L", 17);
    tree.addElem("LR", 150);
    tree.addElem("LRM", -15);
    tree.addElem("RR", 43);
    tree.addElem("RRR", 44);
    tree.addElem("RRRR", 44);
    tree.addElem("RRRRR", 44);
    tree.addElem("M", 50);*/

    std::cout << tree.convertToStringB() << std::endl;
    TernaryTree<int> tree2 = convertFromStringB<int>(tree.convertToStringB());
    tree2.print();
    tree2.addElem("", 15);
    tree2.getHeight();
    tree2.print();
    tree2.deleteNode("");
    tree2.print();
}
