#ifndef LAB3_NODE_HPP
#define LAB3_NODE_HPP

#include "iostream"

template <class T> class Node;
template <class T> bool operator==(const Node<T>& lop, const Node<T>& rop);

template <class T>
class Node
{
private:
    T m_Value = T();
    std::string m_Path;

public:
    Node<T>* left = nullptr;
    Node<T>* middle = nullptr;
    Node<T>* right = nullptr;


    explicit Node(const T& _value, const std::string& _coordinates);
    ~Node() = default;
    T& getValue();
    const std::string& getPath();
    void setValue(const T& _value);

    Node<T>& operator= (const Node<T>& other);

    friend bool operator== <> (const Node<T>& lop, const Node<T>& rop);
};

template <class T>
Node<T>::Node(const T& _value, const std::string& _coordinates)
    : m_Value(_value), m_Path(_coordinates) {}

template <class T>
T& Node<T>::getValue(){ return m_Value; }

template <class T>
void Node<T>::setValue(const T& _value) { m_Value = _value; }

template <class T>
const std::string& Node<T>::getPath(){ return m_Path; }

template <class T>
void printNode(Node<T>* node){ std::cout << node->getValue() << " "; }

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
    m_Path = other.m_Path;

    if (!left && other.left)
        left = new Node<T>(T(), "");
    if (left && !other.left)
    {
        deleteBranch(left);
        left = nullptr;
    }
    if (other.left)
        *left = *(other.left);

    if (!middle && other.middle)
        middle = new Node<T>(T(), "");
    if (middle && !other.middle)
    {
        deleteBranch(middle);
        middle = nullptr;
    }
    if (other.middle)
        *middle = *(other.middle);

    if (!right && other.right)
        right = new Node<T>(T(), "");
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
    return
            (lop.m_Value == rop.m_Value)
            && ((!lop.left && !rop.left) || (lop.left && rop.left && *(lop.left) == *(rop.left)))
            && ((!lop.middle && !rop.middle) || (lop.middle && rop.middle && *(lop.middle) == *(rop.middle)))
            && ((!lop.right && !rop.right) || (lop.right && rop.right && *(lop.right) == *(rop.right)));
}

#endif
