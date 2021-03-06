#pragma once

#include "AbstractTree.h"

namespace sine {
namespace tree {

enum Traversal {
    preOrder, inOrder, postOrder
};

template<class T>
class BinaryTree : public virtual AbstractTree<T> {

public:

    BinaryTree();
    BinaryTree(const BinaryTree<T> &);
    virtual ~BinaryTree();

    typedef void(*handler)(ref);
    typedef void(*const_handler)(const_ref);

    void traverse(handler, Traversal);
    void traverse(const_handler, Traversal) const;

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
        static void removeBT(Bnode_ptr);
    };

    Bnode_ptr root;

private:

    void recursiveScan(handler, Bnode_ptr, Traversal);
    void recursiveScan(const_handler, Bnode_ptr, Traversal) const;

};

template<class T>
BinaryTree<T>::BinaryTree() {
    root = NULL;
}

template<class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &o) {
    if (o.root != NULL)
        root = o.root->clone();
}

template<class T>
BinaryTree<T>::~BinaryTree() {
    BinaryNode::removeBT(root);
}

template<class T>
void BinaryTree<T>::traverse(handler h, Traversal o) {
    recursiveScan(h, root, o);
}

template<class T>
void BinaryTree<T>::traverse(const_handler h, Traversal o) const {
    recursiveScan(h, root, o);
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
void BinaryTree<T>::BinaryNode::removeBT(Bnode_ptr root) {
    if (root == NULL)
        return;
    removeBT(root->child[0]);
    removeBT(root->child[1]);
    delete root;
}

template<class T>
void BinaryTree<T>::recursiveScan(handler h, Bnode_ptr root, Traversal o) {
    if (root == NULL)
        return;
    if (o == preOrder)
        h(root->v);
    recursiveScan(h, root->child[0], o);
    if (o == inOrder)
        h(root->v);
    recursiveScan(h, root->child[1], o);
    if (o == postOrder)
        h(root->v);
}

template<class T>
void BinaryTree<T>::recursiveScan
(const_handler h, Bnode_ptr root, Traversal o) const {
    if (root == NULL)
        return;
    if (o == preOrder)
        h(root->v);
    recursiveScan(h, root->child[0], o);
    if (o == inOrder)
        h(root->v);
    recursiveScan(h, root->child[1], o);
    if (o == postOrder)
        h(root->v);
}

}
}
