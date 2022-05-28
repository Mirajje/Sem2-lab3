#include "menu.h"
#include "TernaryTreeTests.h"
#include <intrin.h>

int main()
{
    TernaryTreeTests::allTestsTogether();
    TernaryTree<int> tree;
    tree.addElem("", 15);
    tree.addElem("R", 15);
    tree.addElem("RR", 15);
    tree.addElem("RRR", 15);
    tree.addElem("RRRR", 15);
    tree.addElem("RRRRR", 15);
    tree.addElem("L", 15);
    tree.addElem("LL", 15);
    tree.addElem("LLL", 15);
    tree.addElem("LLLL", 15);
    tree.addElem("LLLLL", 15);
    tree.addElem("M", 15);
    tree.addElem("MM", 15);
    tree.addElem("MMM", 15);

    int64_t t1 = __rdtsc();
    convertFromStringA<int>(tree.convertToStringA("BLMR"), "BLMR");
    int64_t t2 = __rdtsc();
    std::cout << "A variation takes " << t2 - t1 << std::endl;

    t1 = __rdtsc();
    convertFromStringB<int>(tree.convertToStringB());
    t2 = __rdtsc();
    std::cout << "B variation takes " << t2 - t1 << std::endl;

    menu();
}
