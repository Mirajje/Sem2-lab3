#ifndef MAIN_CPP_TERNARYTREETESTS_H
#define MAIN_CPP_TERNARYTREETESTS_H

#include "TernaryTree.hpp"
#include <cassert>

class TernaryTreeTests
{
public:
    static void allTestsTogether();

private:
    static void addElemTest();
    static void equalityTest();
    static void oneEqualsAnotherTest();
    static void oneEqualsAnotherRvalueReferenceTest();
    static void constructorFromOtherTest();
    static void constructorFromOtherRvalueReferenceTest();
    static void getSubTreeTest();
    static void checkForSubTreeTest();
    static void findTest();
    static void deleteNodeTest();
    static void reduceTest();
    static void mapTest();

};

#endif
