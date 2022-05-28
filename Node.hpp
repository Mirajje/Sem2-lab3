#ifndef LAB3_NODE_HPP
#define LAB3_NODE_HPP

#include "iostream"
#include "Errors.h"
#include <map>
#include <vector>

template <class T>
T&& stoElem(const std::string& str) // add for each new type
{
    auto* result = new T();
    if (str.find('.') != std::string::npos)
        *result =  std::stod(str);
    else
        *result = std::stoi(str);

    return std::move(*result);
}

template <class T> class Node;
template <class T> bool operator==(const Node<T>& lop, const Node<T>& rop);

template <class T>
class Node
{
private:
    T m_Value = T();

public:
    Node<T>* left = nullptr;
    Node<T>* middle = nullptr;
    Node<T>* right = nullptr;


    explicit Node(const T& _value);
    ~Node() = default;
    T& getValue();
    void setValue(const T& _value);

    Node<T>& operator= (const Node<T>& other);

    friend bool operator== <> (const Node<T>& lop, const Node<T>& rop);
};

template <class T>
Node<T>::Node(const T& _value)
    : m_Value(_value) {}

template <class T>
T& Node<T>::getValue(){ return m_Value; }

template <class T>
void Node<T>::setValue(const T& _value) { m_Value = _value; }

template <class T>
void printNode(Node<T>* node){ std::cout << node->getValue() << " "; }

template <class T>
Node<T>* convertFromStringNodeA(const std::string& s, const std::string& traversal, int& index, const std::string& path) // only starting with B are working
{
    if (index >= s.size())
        throw Errors(Errors::INVALID_STRING_ERROR);

    auto* node = new Node<T>(-1);

    for (int i = 0; i < 4; i++)
    {
        if (traversal[i] == 'B')
        {
            std::string elem;
            while (index < s.size() && s[index] != '(' && s[index] != ')')
            {
                if (s[index] == '~')
                {
                    index+=1;
                    while (s[index] == ')')
                        index++;
                    delete node;
                    return nullptr;
                }
                elem.push_back(s[index]);
                index++;
            }
            node->setValue(stoElem<T>(elem));
            while (s[index] == ')')
                index++;
        }
        else
            if (index < s.size() && s[index] == '(')
            {
                index++;
                if (traversal[i] == 'L')
                    node->left = convertFromStringNodeA<T>(s, traversal, index, path + 'L');
                else if (traversal[i] == 'M')
                    node->middle = convertFromStringNodeA<T>(s, traversal, index, path + 'M');
                else if (traversal[i] == 'R')
                    node->right = convertFromStringNodeA<T>(s, traversal, index, path + 'R');
            }
    }

    return node;
}

template <class T>
void deleteBranch(Node<T>* current)
{
    if (!current)
        return;

    deleteBranch(current->left);
    deleteBranch(current->middle);
    deleteBranch(current->right);
    delete current;
}

template <class T>
Node<T>& Node<T>::operator= (const Node<T>& other)
{
    m_Value = other.m_Value;

    if (!left && other.left)
        left = new Node<T>(T());
    if (left && !other.left)
    {
        deleteBranch(left);
        left = nullptr;
    }
    if (other.left)
        *left = *(other.left);

    if (!middle && other.middle)
        middle = new Node<T>(T());
    if (middle && !other.middle)
    {
        deleteBranch(middle);
        middle = nullptr;
    }
    if (other.middle)
        *middle = *(other.middle);

    if (!right && other.right)
        right = new Node<T>(T());
    if (right && !other.right)
    {
        deleteBranch(right);
        right = nullptr;
    }
    if (other.right)
        *right = *(other.right);

    return *this;
}

template <class T>
bool operator==(const Node<T>& lop, const Node<T>& rop)
{
    return  (lop.m_Value == rop.m_Value)
            && ((!lop.left && !rop.left) || (lop.left && rop.left && *(lop.left) == *(rop.left)))
            && ((!lop.middle && !rop.middle) || (lop.middle && rop.middle && *(lop.middle) == *(rop.middle)))
            && ((!lop.right && !rop.right) || (lop.right && rop.right && *(lop.right) == *(rop.right)));
}

#endif
