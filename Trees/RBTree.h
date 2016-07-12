#pragma once

#include <stdexcept>
#include <cassert>
#include "BinaryTree.h"

namespace sine {
namespace tree {

template<class T>
class RBTree : public BinaryTree<T> {

public:

    RBTree();
    RBTree(const RBTree<T> &);
    virtual ~RBTree();

    virtual bool insert(const_ref);
    virtual bool remove(const_ref);

    virtual bool find(ref);

    int test();

private:

    class Node;
    typedef Node * node_ptr;
    typedef Node *& node_ptr_ref;

    class Node {
    public:
        T v;
        bool red;
        node_ptr child[2];
        Node();
        Node(const_ref);
        static node_ptr clone(node_ptr);
        static void remove(node_ptr_ref);
    };

    static bool insertToTree(const_ref, node_ptr_ref, int &sign);
    //static bool removeFromTree(const_ref, node_ptr_ref);

    static bool findInTree(ref, node_ptr);

    static void rotate(node_ptr_ref, bool right);
    //static void fillNode(node_ptr_ref);

    static int testAndGetBlacks(node_ptr);

    node_ptr root;

};

template<class T>
RBTree<T>::RBTree() {
    root = NULL;
}

template<class T>
RBTree<T>::RBTree(const RBTree<T> &o) {
    root = Node::clone(o.root);
}

template<class T>
RBTree<T>::~RBTree() {
    Node::remove(root);
}

template<class T>
bool RBTree<T>::insert(const_ref t) {
    if (root == NULL) {
        root = new Node(t);
        root->red = false;
        return true;
    }
    int unused;
    if (!insertToTree(t, root, unused))
        return false;
    root->red = false;
    return true;
}

template<class T>
bool RBTree<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    return false;
    //return removeFromTree(t, root);
}

template<class T>
bool RBTree<T>::find(ref r) {
    return findInTree(r, root);
}

template<class T>
int RBTree<T>::test() {
    return testAndGetBlacks(root);
}

template<class T>
RBTree<T>::Node::Node()
    : red(true) {
    memset(child, NULL, 2 * sizeof(node_ptr));
}

template<class T>
RBTree<T>::Node::Node(const_ref v)
    : v(v), red(true) {
    memset(child, NULL, 2 * sizeof(node_ptr));
}

template<class T>
typename RBTree<T>::node_ptr RBTree<T>::Node::clone(node_ptr root) {
    if (root == NULL)
        return NULL;
    node_ptr rtn = new node_ptr(root->v);
    rtn->child[0] = clone(root->child[0]);
    rtn->child[1] = clone(root->child[1]);
    rtn->BF = root->BF;
    return rtn;
}

template<class T>
void RBTree<T>::Node::remove(node_ptr_ref root) {
    if (root == NULL)
        return;
    remove(root->child[0]);
    remove(root->child[1]);
    delete root;
    root = NULL;
}

template<class T>
bool RBTree<T>::insertToTree(const_ref v, node_ptr_ref r, int &sign) {
    if (v == r->v)
        return false;
    int i = v < r->v ? 0 : 1;
    node_ptr_ref c = r->child[i];
    if (c == NULL) {
        c = new Node(v);
        if (r->red)
            sign = i;
        return true;
    }
    int sign2 = -1;
    if (!insertToTree(v, c, sign2))
        return false;
    if (sign2 != -1) {
        if (sign2 != i)
            rotate(c, i == 1);
        c->child[i]->red = false;
        rotate(r, i == 0);
    }
    else if (r->red && c->red)
        sign = i;
    return true;
}

// template<class T>
// bool RBTree<T>::removeFromTree(const_ref v, node_ptr_ref r) {
//     if (v == r->v) {
//         node_ptr del = r;
//         fillNode(r);
//         delete del;
//         return true;
//     }
//     int a = v < r->v ? 1 : -1;
//     int i = v < r->v ? 0 : 1;
//     node_ptr_ref c = r->child[i];
//     if (c == NULL)
//         return false;
//     int BF = c->BF;
//     if (!removeFromTree(v, c))
//         return false;
//     if (c != NULL && (BF == 0 || c->BF != 0))
//         return true;
//     r->BF -= a;
//     if (r->BF * a < -1) {
//         if (r->child[1 - i]->BF * a > 0)
//             rotate(r->child[1 - i], i == 0);
//         rotate(r, i == 1);
//     }
//     return true;
// }

template<class T>
void RBTree<T>::rotate(node_ptr_ref r, bool right) {
    int i = right ? 1 : 0;
    node_ptr c = r->child[1 - i];
    r->child[1 - i] = c->child[i];
    c->child[i] = r;
    r = c;
}

// template<class T>
// void RBTree<T>::fillNode(node_ptr_ref r) {
//     int i = 0;
//     if (r->BF < 0)
//         i = 1;
//     else if (r->BF == 0) {
//         if (r->child[0] == NULL) {
//             r = NULL;
//             return;
//         }
//         i = rand() % 2;
//     }
//     int a = i == 0 ? 1 : -1;
//     int BF = r->BF;
//     node_ptr p = r->child[i];
//     node_ptr_ref c = r->child[i];
//     int SBF = c->BF;
//     fillNode(c);
//     if (c == NULL || (SBF != 0 && c->BF == 0))
//         BF -= a;
//     p->BF = BF;
//     p->child[i] = r->child[i];
//     p->child[1 - i] = r->child[1 - i];
//     r = p;
// }

template<class T>
bool RBTree<T>::findInTree(ref v, node_ptr root) {
    if (root == NULL)
        return false;
    if (v == root->v) {
        v = root->v;
        return true;
    }
    int i = v < root->v ? 0 : 1;
    return findInTree(v, root->child[i]);
}

template<class T>
int RBTree<T>::testAndGetBlacks(node_ptr r) {
    if (r == NULL)
        return 0;
    int b0 = testAndGetBlacks(r->child[0]);
    if (b0 == -1)
        return -1;
    int b1 = testAndGetBlacks(r->child[1]);
    if (b1 == -1)
        return -1;
    if (b0 != b1)
        return -1;
    return b0 + (r->red ? 0 : 1);
}

}
}
