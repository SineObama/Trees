#pragma once

#include <stdexcept>

namespace sine {
namespace tree {

template<class T, std::size_t maxChild>
class AbstractTree {

public:

    typedef Node * Ptr;
    typedef Node *& Ref;
    typedef void(*treat)(Ptr);

    class Node {
    public:
        Ptr child[maxChild];
        T v;
        Node();
        Ref operator[](int index);
    };

    AbstractTree();
    AbstractTree(AbstractTree<T, maxChild>);

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
