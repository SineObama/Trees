#pragma once

#include <stdexcept>

namespace sine {
namespace tree {

template<class T, std::size_t maxChild>
class AbstractTree {

public:


    class Node {
    public:
        Node * child[maxChild];
        T v;
        Node();
        Node *& operator[](int index);
    };

    typedef Node * Ptr;
    typedef Node *& Ref;
    typedef void(*treat)(Ptr);

    AbstractTree();

protected:

    Ptr root;

private:



};

template<class T, int maxChild>
AbstractTree<T, maxChild>::Node::Node() {
    for (std::size_t i = 0; i < maxChild; i++)
        child[i] = NULL;
}

template<class T, int maxChild>
typename AbstractTree<T, maxChild>::Ref
AbstractTree<T, maxChild>::Node::operator[](int index) {
    if (index >= maxChild)
        throw std::out_of_range();
    return *child[index];
}

template<class T, int maxChild>
AbstractTree<T, maxChild>::AbstractTree() {
    root = NULL;
}

}
}
