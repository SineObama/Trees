#pragma once

#include "AbstractTree.h"

namespace sine {
namespace tree {

template<class T>
class BinaryTree : public AbstractTree<T> {

public:

    typedef void(*handler)(ref);
    typedef void(*const_handler)(const_ref);

    void preOrder(handler);

protected:

    class BinaryNode;
    typedef BinaryNode * Bnode_ptr;
    typedef BinaryNode *& Bnode_ptr_ref;

    class BinaryNode {
    public:
        T v;
        Bnode_ptr child[2];
        BinaryNode();
        BinaryNode(const_ref);
        virtual ~BinaryNode();
        virtual Bnode_ptr clone();
        static void remove(Bnode_ptr);
    };

    Bnode_ptr root;

private:

    void recursivePreOrder(handler, Bnode_ptr);

};

template<class T>
void BinaryTree<T>::preOrder(handler h) {
    recursivePreOrder(h, root);
}

template<class T>
BinaryTree<T>::BinaryNode::BinaryNode() {
    memset(child, NULL, 2 * sizeof(Bnode_ptr));
}

template<class T>
BinaryTree<T>::BinaryNode::BinaryNode(const_ref v)
    : v(v) {
    memset(child, NULL, 2 * sizeof(Bnode_ptr));
}

template<class T>
BinaryTree<T>::BinaryNode::~BinaryNode() {
}

template<class T>
typename BinaryTree<T>::Bnode_ptr BinaryTree<T>::BinaryNode::clone() {
    Bnode_ptr rtn = new BinaryNode(v);
    if (child[0] != NULL)
        rtn->child[0] = child[0]->clone();
    if (child[1] != NULL)
        rtn->child[1] = child[1]->clone();
    return rtn;
}

template<class T>
void BinaryTree<T>::BinaryNode::remove(Bnode_ptr root) {
    if (root == NULL)
        return;
    remove(root->child[0]);
    remove(root->child[1]);
    delete root;
}

template<class T>
void BinaryTree<T>::recursivePreOrder(handler h, Bnode_ptr root) {
    if (root == NULL)
        return;
    h(root);
    recursivePreOrder(handler, root->child[0]);
    recursivePreOrder(handler, root->child[1]);
}

}
}
