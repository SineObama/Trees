#pragma once

#include <stdexcept>
#include "BinaryTree.h"

namespace sine {
namespace tree {

template<class T>
class AVLTree {

public:

    typedef const T & const_ref

    AVLTree();
    AVLTree(const AVLTree<T> &);
    ~AVLTree();

    bool insert(const_ref);
    bool remove(const_ref);


private:

    typedef Node * NodePtr;
    typedef Node *& NodeRef;

    class Node {
        T v;
        NodePtr child[2];
        int BF;
        Node();
        Node(const_ref);
        NodeRef operator[](int index);
        static NodePtr clone(NodePtr);
        static void remove(NodeRef);
    };

    bool insertToNode(const_ref, NodeRef);
    bool removeFromNode(const_ref, NodeRef);

    NodePtr root;

};

template<class T>
AVLTree<T>::AVLTree() {
    root = NULL;
}

template<class T>
AVLTree<T>::AVLTree(const AVLTree<T> &o) {
    root = Node::clone(o.root);
}

template<class T>
AVLTree<T>::~AVLTree() {
    Node::remove(root);
}

template<class T>
bool AVLTree<T>::insert(const_ref t) {
    return insertToNode(t, root);
}

template<class T>
bool AVLTree<T>::remove(const_ref t) {
    return removeFromNode(t, root);
}

template<class T>
AVLTree<T>::Node::Node() {
    for (std::size_t i = 0; i < maxChild; i++)
        child[i] = NULL;
}

template<class T>
AVLTree<T>::Node::Node(const_ref t)
    : v(t) {
    for (std::size_t i = 0; i < maxChild; i++)
        child[i] = NULL;
}

template<class T>
typename AbstractTree<T>::NodeRef
AbstractTree<T>::Node::operator[](int index) {
    if (index >= 2)
        throw std::out_of_range();
    return *child[index];
}

template<class T>
typename AbstractTree<T>::NodePtr AbstractTree<T>::Node::clone(NodePtr root) {
    if (root == NULL)
        return NULL;
    NodePtr rtn = new NodePtr(root->v);
    rtn->child[0] = clone(root->child[0]);
    rtn->child[1] = clone(root->child[1]);
    rtn->BF = root->BF;
    return rtn;
}

template<class T>
void AbstractTree<T>::Node::remove(NodeRef root) {
    if (root == NULL)
        return NULL;
    remove(root->child[0]);
    remove(root->child[1]);
    delete root;
    root = NULL;
}

template<class T>
bool AVLTree<T>::insertToNode(const_ref v, NodeRef r) {

}

template<class T>
bool AVLTree<T>::removeFromNode(const_ref v, NodeRef r) {
    if (r == NULL)
        return false;
}

}
}
