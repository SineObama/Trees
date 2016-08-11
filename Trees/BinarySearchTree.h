#pragma once

#include "BinaryTree.h"
#include "SearchTree.h"

namespace sine {
namespace tree {

/**
 * 二叉查找树，小的放左大的放右。
 */
template<class T>
class BinarySearchTree
    : public virtual BinaryTree<T>, public virtual SearchTree<T> {

public:

     ptr find(const_ref);
     const_ptr find(const_ref) const;

     bool checkValid() const;

private:

    static ptr findInTree(const_ref, Bnode_ptr);

    static bool checkValidRecursive(Bnode_ptr);

};

template<class T>
typename BinarySearchTree<T>::ptr BinarySearchTree<T>::find(const_ref r) {
    return findInTree(r, root);
}

template<class T>
typename BinarySearchTree<T>::const_ptr BinarySearchTree<T>::find
(const_ref r) const {
    return findInTree(r, root);
}

template<class T>
bool BinarySearchTree<T>::checkValid() const {
    return checkValidRecursive(root);
}

template<class T>
typename BinarySearchTree<T>::ptr BinarySearchTree<T>::findInTree
(const_ref v, Bnode_ptr root) {
    if (root == NULL)
        return NULL;
    if (v == root->v)
        return &root->v;
    int i = v < root->v ? 0 : 1;
    return findInTree(v, root->child[i]);
}

template<class T>
bool BinarySearchTree<T>::checkValidRecursive(Bnode_ptr _r) {
    if (_r != NULL) {
        if (_r->child[0] != NULL)
            if (!(checkValidRecursive(_r->child[0]) && _r->child[0]->v < _r->v))
                return false;
        if (_r->child[1] != NULL)
            if (!(checkValidRecursive(_r->child[1]) && _r->v < _r->child[1]->v))
                return false;
    }
    return true;
}

}
}
