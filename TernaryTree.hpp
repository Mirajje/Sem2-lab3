#ifndef LAB3_TERNARYTREE_HPP
#define LAB3_TERNARYTREE_HPP

#include "Node.hpp"
#include <stack>
#include <iostream>
#include "Errors.h"
#include <map>
#include <algorithm>
#include <vector>

template <class T>
class TernaryTree
{
private:
    Node<T>* m_Head = nullptr;
    std::string m_Traversal = "BLMR"; // base - left - middle - right

public:
    TernaryTree() = default;
    TernaryTree(const TernaryTree<T>& other);
    TernaryTree(TernaryTree<T>&& other) noexcept;

    const T& getElem(const std::string& path);
    void setElem(const T& item, const std::string& path);
    bool exists(const std::string& path); // checks if path leads to a Node

    bool operator == (const TernaryTree<T>&);
    TernaryTree<T>& operator =(const TernaryTree<T>& other);
    TernaryTree<T>& operator =(TernaryTree<T>&& other) noexcept;
    TernaryTree<T>&& getSubTree(const std::string& path);
    bool checkForSubTree(const TernaryTree<T>& tree);

    std::string&& convertToString(std::string traversal = "BLMR");

    void addElem(const std::string& path, const T& value = 0); // path - capital letters only
    std::string find(const T& value); // returns None if nothing was found
    void deleteNode(const std::string& path); // deletes Node and a branch under it
    T&& reduce(const T& startingValue, T func(const T& value1, const T& value2));
    void map(void func(T& value));
    void changeTraversal(const std::string& _traversal); // permutation of "BLMR" capital letters only
    void print();

};

template <class T> Node<T>* getNode(Node<T>* head, const std::string& path);
template <class T> void mapRecursion(const std::string& traversal, Node<T>* current, void func(T& value));
template <class T> void reduceRecursion(const std::string& traversal, Node<T>* current, T* sum, T func(const T& value1, const T& value2));

template <class T>
TernaryTree<T>::TernaryTree(const TernaryTree<T>& other)
{
    *this = other;
}

template <class T>
TernaryTree<T>::TernaryTree(TernaryTree<T>&& other) noexcept
{
    *this = std::move(other);
}

template <class T>
const T& TernaryTree<T>::getElem(const std::string& path)
{
    return getNode(m_Head, path)->getValue();
}

template <class T>
void TernaryTree<T>::setElem(const T& item, const std::string& path)
{
    getNode(m_Head, path)->setValue(item);
}

template <class T>
bool TernaryTree<T>::exists(const std::string& path)
{
    Node<T>* current = m_Head;
    for (char i : path)
    {
        if (i == 'L')
        {
            if (!current->left)
                return false;

            current = current->left;
        }
        else if (i == 'M')
        {
            if (!current->middle)
                return false;

            current = current->middle;
        }
        else if (i == 'R')
        {
            if (!current->right)
                return false;

            current = current->right;
        }
        else
            throw Errors(Errors::WRONG_PATH_ERROR);
    }
    return true;
}

template <class T>
bool TernaryTree<T>::operator == (const TernaryTree<T>& other)
{
    return (!m_Head && !(other.m_Head)) || (m_Head && other.m_Head && *m_Head == *(other.m_Head));
}

template <class T>
TernaryTree<T>& TernaryTree<T>::operator = (const TernaryTree<T>& other)
{
    if (this != &other)
    {
        deleteBranch(m_Head);
        if (!other.m_Head)
            m_Head = nullptr;
        else
        {
            m_Head = new Node<T> (T(), "");
            *m_Head = *(other.m_Head);
        }
    }

    return *this;
}

template <class T>
TernaryTree<T>& TernaryTree<T>::operator = (TernaryTree<T>&& other) noexcept
{
    if (this != &other)
    {
        deleteBranch(m_Head);
        m_Head = other.m_Head;
        other.m_Head = nullptr;
    }

    return *this;
}

template <class T>
TernaryTree<T>&& TernaryTree<T>::getSubTree(const std::string& path)
{
    auto* result = new TernaryTree<T>;
    result->m_Head = new Node<T>(T(), "");
    *(result->m_Head) = *getNode(m_Head, path);
    return std::move(*result);
}

template <class T>
bool TernaryTree<T>::checkForSubTree(const TernaryTree<T>& tree)
{
    if (!tree.m_Head)
        return true;

    std::stack<Node<T>*> stack;
    Node<T>* current;
    stack.push(m_Head);
    while (!stack.empty())
    {
        current = stack.top();
        stack.pop();

        if (*current == (*tree.m_Head))
            return true;

        if (current->right)
            stack.push(current->right);

        if (current->middle)
            stack.push(current->middle);

        if (current->left)
            stack.push(current->left);
    }

    return false;
}

template <class T>
std::string&& TernaryTree<T>::convertToString(std::string traversal)
{
    if (traversal == "null")
        traversal = m_Traversal;

    auto* result = new std::string;
    convertRecursion(traversal, m_Head, result);
    return std::move(*result);
}

template <class T>
void TernaryTree<T>::addElem(const std::string& path, const T& value)
{
    if (m_Head == nullptr)
    {
        if (path.size() > 1)
            throw Errors(Errors::WRONG_PATH_ERROR);

        m_Head = new Node<T>(value, "");
        return;
    }

    Node<T>* current = getNode(m_Head, path.substr(0, path.size() - 1));

    if (path[path.size() - 1] == 'L' && !current->left)
        current->left = new Node<T>(value, path);

    else if (path[path.size() - 1] == 'M' && !current->middle)
        current->middle = new Node<T>(value, path);

    else if (path[path.size() - 1] == 'R' && !current->right)
        current->right = new Node<T>(value, path);

    else
        throw Errors(Errors::WRONG_PATH_ERROR);

}

template <class T>
void TernaryTree<T>::changeTraversal(const std::string &_traversal)
{
    if (_traversal.size() != 4)
        throw Errors(Errors::WRONG_TRAVERSAL_INPUT_ERROR);

    std::vector<bool> vec(4, false); // "BLMR"
    for (int i = 0; i < 4; i++)
    {
        if (_traversal[i] == 'B')
            vec[0] = true;
        else if (_traversal[i] == 'L')
            vec[1] = true;
        else if (_traversal[i] == 'M')
            vec[2] = true;
        else if (_traversal[i] == 'R')
            vec[3] = true;
    }

    if (!(vec[0] && vec[1] && vec[2] && vec[3]))
        throw Errors(Errors::WRONG_TRAVERSAL_INPUT_ERROR);

    m_Traversal = _traversal;

}

template <class T>
void TernaryTree<T>::map(void func(T& value))
{
    mapRecursion(m_Traversal, m_Head, func);
}

template <class T>
T&& TernaryTree<T>::reduce(const T& startingValue, T func(const T& value1, const T& value2))
{
    T* sum = new T(startingValue);
    reduceRecursion(m_Traversal, m_Head, sum, func);
    return std::move(*sum);
}

template <class T>
std::string TernaryTree<T>::find(const T& value)
{
    std::stack<Node<T>*> stack;
    Node<T>* current;
    stack.push(m_Head);
    while (!stack.empty())
    {
        current = stack.top();
        stack.pop();

        if (current->getValue() == value)
            return current->getPath();

        if (current->right)
            stack.push(current->right);

        if (current->middle)
            stack.push(current->middle);

        if (current->left)
            stack.push(current->left);

    }

    return "None";
}

template <class T>
void TernaryTree<T>::deleteNode(const std::string& path)
{
    if (!m_Head)
        throw Errors(Errors::ZERO_SIZE_ERROR);

    Node<T>* current = getNode(m_Head, path.substr(0, path.size() - 1));

    if (path.empty())
    {
        deleteBranch(current);
        m_Head = nullptr;
    }
    else if (path[path.size() - 1] == 'L' && current->left)
    {
        deleteBranch(current->left);
        current->left = nullptr;
    }
    else if (path[path.size() - 1] == 'M' && current->middle)
    {
        deleteBranch(current->middle);
        current->middle = nullptr;
    }
    else if (path[path.size() - 1] == 'R' && current->right)
    {
        deleteBranch(current->right);
        current->right = nullptr;
    }
    else
        throw Errors(Errors::WRONG_PATH_ERROR);
}

template <class T>
void TernaryTree<T>::print()
{
    mapRecursion<T>(m_Traversal, m_Head, [](T& value) { std::cout << value << " "; });
    std::cout << std::endl;
}

template <class T>
Node<T>* getNode(Node<T>* head, const std::string& path)
{
    Node<T>* current = head;
    for (char i : path)
    {
        if (i == 'L')
        {
            if (!current->left)
                throw Errors(Errors::WRONG_PATH_ERROR);

            current = current->left;
        }
        else if (i == 'M')
        {
            if (!current->middle)
                throw Errors(Errors::WRONG_PATH_ERROR);

            current = current->middle;
        }
        else if (i == 'R')
        {
            if (!current->right)
                throw Errors(Errors::WRONG_PATH_ERROR);

            current = current->right;
        }
        else
            throw Errors(Errors::WRONG_PATH_ERROR);
    }
    return current;
}

template <class T>
void convertRecursion(const std::string& traversal, Node<T>* current, std::string* result)
{
    if (!current)
    {
        for (int i = 0; i < 4; i++)
            result->push_back("null"[i]);
        result->push_back('~');
        return;
    }

    std::map<char, Node<T>*> dict;
    dict['L'] = current->left;
    dict['M'] = current->middle;
    dict['R'] = current->right;

    for (int i = 0; i < 4; i++)
        if (traversal[i] == 'B')
        {
            for (char c : std::to_string(current->getValue()))
                result->push_back(c);
            result->push_back('~');
        }
        else
            convertRecursion(traversal, dict[traversal[i]], result);
}

template <class T>
void mapRecursion(const std::string& traversal, Node<T>* current, void func(T& value))
{
    if (!current)
        return;

    std::map<char, Node<T>*> dict;
    dict['L'] = current->left;
    dict['M'] = current->middle;
    dict['R'] = current->right;

    for (int i = 0; i < 4; i++)
        if (traversal[i] == 'B')
            func(current->getValue());
        else
            mapRecursion(traversal, dict[traversal[i]], func);
}

template <class T>
void reduceRecursion(const std::string& traversal, Node<T>* current, T* sum, T func(const T& value1, const T& value2))
{
    if (!sum)
        throw Errors(Errors::NULL_POINTER_ERROR);

    if (!current)
        return;

    std::map<char, Node<T>*> dict;
    dict['L'] = current->left;
    dict['M'] = current->middle;
    dict['R'] = current->right;

    for (int i = 0; i < 4; i++)
        if (traversal[i] == 'B')
            *sum = func(current->getValue(), *sum);
        else
            reduceRecursion(traversal, dict[traversal[i]], sum, func);
}

#endif
