#pragma once

#include <stdexcept>
#include "BinaryTree.h"

namespace sine {
namespace tree {

template<class T>
class AVLTree : public BinaryTree<T> {

public:

    AVLTree();
    AVLTree(const AVLTree<T> &);
    virtual ~AVLTree();

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
        int BF;
        node_ptr child[2];
        Node();
        Node(const_ref);
        static node_ptr clone(node_ptr);
        static void remove(node_ptr_ref);
    };

    static bool insertToTree(const_ref, node_ptr_ref);
    static bool removeFromTree(const_ref, node_ptr_ref);

    static bool findInTree(ref, node_ptr);

    static void rotate(node_ptr_ref, bool right);
    static void fillNode(node_ptr_ref);

    static int testAndGetHeight(node_ptr);

    node_ptr root;

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
    if (root == NULL) {
        root = new Node(t);
        return true;
    }
    return insertToTree(t, root);
}

template<class T>
bool AVLTree<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    return removeFromTree(t, root);
}

template<class T>
bool AVLTree<T>::find(ref r) {
    return findInTree(r, root);
}

template<class T>
int AVLTree<T>::test() {
    return testAndGetHeight(root);
}

template<class T>
AVLTree<T>::Node::Node()
    : BF(0) {
    memset(child, NULL, 2 * sizeof(node_ptr));
}

template<class T>
AVLTree<T>::Node::Node(const_ref v)
    : v(v), BF(0) {
    memset(child, NULL, 2 * sizeof(node_ptr));
}

template<class T>
typename AVLTree<T>::node_ptr AVLTree<T>::Node::clone(node_ptr root) {
    if (root == NULL)
        return NULL;
    node_ptr rtn = new node_ptr(root->v);
    rtn->child[0] = clone(root->child[0]);
    rtn->child[1] = clone(root->child[1]);
    rtn->BF = root->BF;
    return rtn;
}

template<class T>
void AVLTree<T>::Node::remove(node_ptr_ref root) {
    if (root == NULL)
        return;
    remove(root->child[0]);
    remove(root->child[1]);
    delete root;
    root = NULL;
}

/**
 * 不接受空节点
 */
template<class T>
bool AVLTree<T>::insertToTree(const_ref v, node_ptr_ref r) {
    if (v == r->v)
        return false;
    int a = v < r->v ? 1 : -1;
    int i = v < r->v ? 0 : 1;
    node_ptr_ref c = r->child[i];
    if (c == NULL) {
        r->BF += a;
        c = new Node(v);
        return true;
    }
    int BF = c->BF;
    if (!insertToTree(v, c))
        return false;
    if (BF != 0 || c->BF == 0)
        return true;
    r->BF += a;
    if (r->BF * a > 1) {
        if (c->BF * a < 0)
            rotate(c, i == 1);
        rotate(r, i == 0);
    }
    return true;
}

/**
* 不接受空节点
*/
template<class T>
bool AVLTree<T>::removeFromTree(const_ref v, node_ptr_ref r) {
    if (v == r->v) {
        node_ptr del = r;
        fillNode(r);
        delete del;
        return true;
    }
    int a = v < r->v ? 1 : -1;
    int i = v < r->v ? 0 : 1;
    node_ptr_ref c = r->child[i];
    if (c == NULL)
        return false;
    int BF = c->BF;
    if (!removeFromTree(v, c))
        return false;
    if (c != NULL && (BF == 0 || c->BF != 0))
        return true;
    r->BF -= a;
    if (r->BF * a < -1) {
        if (r->child[1 - i]->BF * a > 0)
            rotate(r->child[1 - i], i == 0);
        rotate(r, i == 1);
    }
    return true;
}

/**
 * 演算：
 * 　　　　ｎ
 * 　　　／　＼
 * 　　ｍ　　　ｚ
 * 　／　＼
 * ｘ　　　ｙ
 * x,y,z是节点深度，m,n是平衡因子。准备右旋。
 * m = x - y, n = Max{x, y} + 1 - z
 * 　　　　m2
 * 　　　／　＼
 * 　　ｘ　　　n2
 * 　　　　　／　\
 * 　　　　ｙ　　　ｚ
 * m2 = x - Max{y, z} - 1, n2 = y - z
 * 分2种情况：
 * 1. x <= y 即 m <= 0
 * m = x - y, n = y - z + 1
 * m2 = Min{m, m + n - 1} - 1, n2 = n - 1
 * 2. x > y 即 m > 0
 * m = x - y, n = x - z + 1;
 * m2 = Min{m, n - 1} - 1, n2 = n - m - 1;
 * 综合：
 * m2 - m = Min{0, n2} - 1, n2 - n = Min{0, -m} - 1
 */
template<class T>
void AVLTree<T>::rotate(node_ptr_ref r, bool right) {
    int i = right ? 1 : 0;
    int a = right ? -1 : 1;
    node_ptr c = r->child[1 - i];
    r->child[1 - i] = c->child[i];
    c->child[i] = r;
    r->BF += a - (a * c->BF < 0 ? c->BF : 0);
    c->BF += a + (a * r->BF > 0 ? r->BF : 0);
    r = c;
}

template<class T>
void AVLTree<T>::fillNode(node_ptr_ref r) {
    int i = 0;
    if (r->BF < 0)
        i = 1;
    else if (r->BF == 0) {
        if (r->child[0] == NULL) {
            r = NULL;
            return;
        }
        i = rand() % 2;
    }
    int a = i == 0 ? 1 : -1;
    int BF = r->BF;
    node_ptr p = r->child[i];
    node_ptr_ref c = r->child[i];
    int SBF = c->BF;
    fillNode(c);
    if (c == NULL || (SBF != 0 && c->BF == 0))
        BF -= a;
    p->BF = BF;
    p->child[i] = r->child[i];
    p->child[1 - i] = r->child[1 - i];
    r = p;
}

template<class T>
bool AVLTree<T>::findInTree(ref v, node_ptr root) {
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
int AVLTree<T>::testAndGetHeight(node_ptr r) {
    if (r == NULL)
        return 0;
    int h0 = testAndGetHeight(r->child[0]);
    if (h0 == -1)
        return -1;
    int h1 = testAndGetHeight(r->child[1]);
    if (h1 == -1)
        return -1;
    if (r->BF * r->BF > 1 || r->BF != h0 - h1)
        return -1;
    return (h0 > h1 ? h0 : h1) + 1;
}

}
}
