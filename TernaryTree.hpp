#ifndef LAB3_TERNARYTREE_HPP_1
#define LAB3_TERNARYTREE_HPP_1

#include "Node.hpp"
#include <stack>
#include <iostream>
#include "Errors.h"
#include <map>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>

template <class T>
class TernaryTree
{
private:
    Node<T>* m_Head = nullptr;
    std::string m_Traversal = "BLMR"; // base - left - middle - right
    mutable int m_Height = 0;
    mutable bool heightUpdated = false;

public:
    TernaryTree() = default;
    TernaryTree(const TernaryTree<T>& other);
    TernaryTree(TernaryTree<T>&& other) noexcept;
    explicit TernaryTree(Node<T>* node);
    ~TernaryTree();

    const T& getElem(const std::string& path) const;
    void setElem(const std::string& path, const T& item);
    int getHeight() const;
    bool exists(const std::string& path) const; // checks if path leads to a Node

    bool operator == (const TernaryTree<T>&) const;
    TernaryTree<T>& operator =(const TernaryTree<T>& other);
    TernaryTree<T>& operator =(TernaryTree<T>&& other) noexcept;
    TernaryTree<T>&& getSubTree(const std::string& path) const;
    bool checkForSubTree(const TernaryTree<T>& tree) const;

    std::string&& convertToStringA(std::string traversal = "null") const;
    std::string&& convertToStringB() const;

    void addElem(const std::string& path, const T& value = 0); // path - capital letters only
    std::string&& find(const T& value) const; // returns None if nothing was found
    void deleteNode(const std::string& path); // deletes Node and a branch under it
    T&& reduce(const T& startingValue, T func(const T& value1, const T& value2)) const;
    void map(void func(T& value));
    void changeTraversal(const std::string& _traversal); // permutation of "BLMR" capital letters only
    void print() const;

};

template <class T> void findRecursion(const T& value, const std::string& path, Node<T>* current, std::string* result);
template <class T> Node<T>* getNode(Node<T>* head, const std::string& path);
template <class T> void updateHeight(Node<T>* current, int& maxHeight, int currentHeight);
template <class T> void mapRecursion(const std::string& traversal, Node<T>* current, void func(T& value));
template <class T> void reduceRecursion(const std::string& traversal, Node<T>* current, T* sum, T func(const T& value1, const T& value2));
template <class T> void convertToStringRecursionA(const std::string& traversal, Node<T>* current, std::string* result);
template <class T> void convertToStringRecursionB(Node<T>* current, std::vector<std::string>* result, int64_t coor);
template <class T> TernaryTree<T>&& convertFromStringA(const std::string& str, const std::string& traversal); // only starting with B are working
template <class T> TernaryTree<T>&& convertFromStringB(const std::string& str);

static std::string&& indexIntoPath(int64_t index)
{
    auto* result = new std::string;
    while (index > 0)
    {
        *result = (index % 3 == 1 ? 'L' : (index % 3 == 2 ? 'M' : 'R')) + *result;
        if (index % 3 == 0)
            index = (index - 3) / 3;
        else
            index = (index - index % 3) / 3;
    }
    return std::move(*result);
}

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
TernaryTree<T>::TernaryTree(Node<T>* node)
{
    m_Head = node;
}

template <class T>
TernaryTree<T>::~TernaryTree()
{
    deleteBranch(m_Head);
}

template <class T>
Node<T>* getNode(Node<T>* head, const std::string& path)
{
    if (!head)
        throw Errors(Errors::NULL_POINTER_ERROR);

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
const T& TernaryTree<T>::getElem(const std::string& path) const
{
    return getNode(m_Head, path)->getValue();
}

template <class T>
void updateHeight(Node<T>* current, int& maxHeight, int currentHeight)
{
    if (!current)
        return;

    if (currentHeight > maxHeight)
        maxHeight = currentHeight;

    updateHeight(current->left, maxHeight, currentHeight + 1);
    updateHeight(current->middle, maxHeight, currentHeight + 1);
    updateHeight(current->right, maxHeight, currentHeight + 1);
}

template <class T>
int TernaryTree<T>::getHeight() const
{
    if (heightUpdated)
        return m_Height;

    int maxHeight = 0;
    updateHeight(m_Head, maxHeight, (m_Head ? 1 : 0));
    heightUpdated = true;
    m_Height = maxHeight;
    return m_Height;
}

template <class T>
void TernaryTree<T>::setElem(const std::string& path, const T& item)
{
    getNode(m_Head, path)->setValue(item);
}

template <class T>
bool TernaryTree<T>::exists(const std::string& path) const
{
    if (!m_Head)
        return false;

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
bool TernaryTree<T>::operator == (const TernaryTree<T>& other) const
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
        {
            m_Head = nullptr;
            m_Height = 0;
        }
        else
        {
            m_Head = new Node<T> (T());
            *m_Head = *(other.m_Head);
            m_Height = other.getHeight();
        }
        heightUpdated = true;
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
        m_Height = other.getHeight();
        other.m_Head = nullptr;
        heightUpdated = true;
    }

    return *this;
}

template <class T>
TernaryTree<T>&& TernaryTree<T>::getSubTree(const std::string& path) const
{
    auto* result = new TernaryTree<T>;
    result->m_Head = new Node<T>(T());
    *(result->m_Head) = *getNode(m_Head, path);

    return std::move(*result);
}

template <class T>
bool TernaryTree<T>::checkForSubTree(const TernaryTree<T>& tree) const
{
    if (!tree.m_Head)
        return true;

    if (!m_Head)
        return false;

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
std::string&& TernaryTree<T>::convertToStringA(std::string traversal) const
{
    if (traversal == "null")
        traversal = m_Traversal;

    if (traversal[0] != 'B')
        throw Errors(Errors::WRONG_TRAVERSAL_INPUT_ERROR);

    auto* result = new std::string;
    convertToStringRecursionA(traversal, m_Head, result);
    return std::move(*result);
}

template <class T>
std::string&& TernaryTree<T>::convertToStringB() const
{
    auto* result = new std::vector<std::string>((pow(3, getHeight()) - 1)/2, "~");
    convertToStringRecursionB(m_Head, result, 0);
    auto* ans = new std::string;
    for (auto & i : *result)
        *ans += (i + '`');
    delete result;
    return std::move(*ans);
}

template <class T>
TernaryTree<T>&& convertFromStringA(const std::string& str, const std::string& traversal) // only starting with B are working
{
    if (traversal[0] != 'B')
        throw Errors(Errors::WRONG_TRAVERSAL_INPUT_ERROR);

    int index = 1;
    auto* result = new TernaryTree<T>(convertFromStringNodeA<T>(str, traversal, index, ""));
    return std::move(*result);
}

template <class T>
TernaryTree<T>&& convertFromStringB(const std::string& str)
{
    int64_t index = 0;
    auto* mas = new std::vector<std::string>;
    std::string current;
    for (char c : str)
    {
        if (c != '`')
            current += c;
        else
        {
            mas->push_back(current);
            index += 1;
            current = "";
        }
    }

    auto* result = new TernaryTree<T>;
    for (int i = 0; i < mas->size(); i++)
    {
        if ((*mas)[i] != "~")
            result->addElem(indexIntoPath(i), stoElem<T>((*mas)[i]));
    }
    return std::move(*result);
}

template <class T>
void TernaryTree<T>::addElem(const std::string& path, const T& value)
{
    heightUpdated = false;
    if (m_Head == nullptr)
    {
        m_Head = new Node<T>(value);
        return;
    }

    Node<T>* current = getNode(m_Head, path.substr(0, path.size() - 1));

    if (path[path.size() - 1] == 'L' && !current->left)
        current->left = new Node<T>(value);

    else if (path[path.size() - 1] == 'M' && !current->middle)
        current->middle = new Node<T>(value);

    else if (path[path.size() - 1] == 'R' && !current->right)
        current->right = new Node<T>(value);

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
T&& TernaryTree<T>::reduce(const T& startingValue, T func(const T& value1, const T& value2)) const
{
    T* sum = new T(startingValue);
    reduceRecursion(m_Traversal, m_Head, sum, func);
    return std::move(*sum);
}

template <class T>
std::string&& TernaryTree<T>::find(const T& value) const
{
    auto* result = new std::string("None");
    findRecursion(value, "", m_Head, result);
    return std::move(*result);
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

    heightUpdated = false;
}

template <class T>
void TernaryTree<T>::print() const
{
    mapRecursion<T>(m_Traversal, m_Head, [](T& value) { std::cout << value << " "; });
    std::cout << std::endl;
}

template <class T>
void convertToStringRecursionA(const std::string& traversal, Node<T>* current, std::string* result)
{
    if (!current)
    {
        result->push_back('(');
        result->push_back('~');
        result->push_back(')');
        return;
    }

    std::map<char, Node<T>*> dict;
    dict['L'] = current->left;
    dict['M'] = current->middle;
    dict['R'] = current->right;

    for (int i = 0; i < 4; i++)
    {
        if (traversal[i] == 'B')
        {
            result->push_back('(');
            for (char c : std::to_string(current->getValue()))
                result->push_back(c);
        }
        else
            convertToStringRecursionA(traversal, dict[traversal[i]], result);
    }
    result->push_back(')');
}

template <class T>
void convertToStringRecursionB(Node<T>* current, std::vector<std::string>* result, int64_t coor)
{
    if (!current)
        return;

    (*result)[coor] = std::to_string(current->getValue());
    convertToStringRecursionB(current->left, result, coor * 3 + 1);
    convertToStringRecursionB(current->middle, result, coor * 3 + 2);
    convertToStringRecursionB(current->right, result, coor * 3 + 3);
}

template <class T>
void findRecursion(const T& value, const std::string& path, Node<T>* current, std::string* result)
{
    if (!current)
        return;

    if (*result != "None")
        return;

    if (current->getValue() == value)
    {
        *result = path;
        return;
    }

    findRecursion(value, path + 'R', current->right, result);
    findRecursion(value, path + 'M', current->middle, result);
    findRecursion(value, path + 'L', current->left, result);
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
