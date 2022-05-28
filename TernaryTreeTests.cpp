#ifndef TERNARY_TREE_TESTS_CPP
#define TERNARY_TREE_TESTS_CPP

#include "TernaryTreeTests.h"

template <class T>
static T reduceTestFunc(const T& v1, const T& v2)
{
    return v1 + v2;
}

template <class T>
static void mapTestFunc(T& value)
{
    value += 1;
}

void TernaryTreeTests::addElemTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    assert(tree1.getElem("") == 15);
    tree1.addElem("R", 16);
    assert(tree1.getElem("R") == 16);
}

void TernaryTreeTests::equalityTest()
{
    TernaryTree<int> tree1;
    TernaryTree<int> tree2;

    tree1.addElem("", 15);
    tree2.addElem("", 15);
    tree1.addElem("R", 16);
    tree2.addElem("R", 16);
    assert(tree1 == tree2);
}

void TernaryTreeTests::findTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    assert(tree1.find(16) == "R");
    assert(tree1.find(15) == "");
    assert(tree1.find(1555) == "None");
}

void TernaryTreeTests::deleteNodeTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    tree1.deleteNode("R");
    assert(!tree1.exists("R"));
}

void TernaryTreeTests::reduceTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    assert(tree1.reduce(19, reduceTestFunc) == 86);
}

void TernaryTreeTests::mapTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    tree1.map(mapTestFunc);
    assert(tree1.reduce(19, reduceTestFunc) == 90);
}

void TernaryTreeTests::oneEqualsAnotherTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    TernaryTree<int> tree2;
    tree2 = tree1;
    assert(tree1 == tree2);
}

void TernaryTreeTests::oneEqualsAnotherRvalueReferenceTest()
{
    TernaryTree<int> tree1;
    TernaryTree<int> tree1Copy;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    tree1Copy = tree1;

    TernaryTree<int> tree2;
    tree2 = std::move(tree1);
    assert(tree1Copy == tree2);
}

void TernaryTreeTests::constructorFromOtherTest()
{
    TernaryTree<int> tree1;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    TernaryTree<int> tree2 = tree1;
    assert(tree1 == tree2);
}

void TernaryTreeTests::constructorFromOtherRvalueReferenceTest()
{
    TernaryTree<int> tree1;
    TernaryTree<int> tree1Copy;

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);
    tree1Copy = tree1;

    TernaryTree<int> tree2 = std::move(tree1);
    assert(tree1Copy == tree2);
}

void TernaryTreeTests::getSubTreeTest()
{
    TernaryTree<int> tree1;
    TernaryTree<int> tree2;

    tree2.addElem("", 17);
    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);

    assert(tree1 == tree1.getSubTree(""));
    assert(tree2 == tree1.getSubTree("M"));
}

void TernaryTreeTests::checkForSubTreeTest()
{
    TernaryTree<int> tree1;
    TernaryTree<int> tree2;

    tree2.addElem("", 148);

    tree1.addElem("", 15);
    tree1.addElem("R", 16);
    tree1.addElem("RR", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);

    assert(tree1.checkForSubTree(tree1.getSubTree("R")));
    assert(!(tree1.checkForSubTree(tree2)));
}

void TernaryTreeTests::convertToStringAndBackTestA()
{
    TernaryTree<int> tree1;
    tree1.addElem("", 148);
    tree1.addElem("R", 16);
    tree1.addElem("RR", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);

    assert(convertFromStringA<int>(tree1.convertToStringA("BLMR"), "BLMR") == tree1);
    assert(convertFromStringA<int>(tree1.convertToStringA("BMLR"), "BMLR") == tree1);
    assert(!(convertFromStringA<int>(tree1.convertToStringA("BMLR"), "BLMR") == tree1));
}

void TernaryTreeTests::convertToStringAndBackTestB()
{
    TernaryTree<int> tree1;
    tree1.addElem("", 148);
    tree1.addElem("R", 16);
    tree1.addElem("RR", 16);
    tree1.addElem("M", 17);
    tree1.addElem("L", 19);

    assert(convertFromStringB<int>(tree1.convertToStringB()) == tree1);
}

void TernaryTreeTests::allTestsTogether()
{
    addElemTest();
    equalityTest();
    oneEqualsAnotherTest();
    oneEqualsAnotherRvalueReferenceTest();
    constructorFromOtherTest();
    constructorFromOtherRvalueReferenceTest();
    getSubTreeTest();
    checkForSubTreeTest();
    findTest();
    deleteNodeTest();
    reduceTest();
    mapTest();
    convertToStringAndBackTestA();
    convertToStringAndBackTestB();

}

#endif