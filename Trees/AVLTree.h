#pragma once

#include <stdexcept>
#include "SelfBalancedBT.h"

namespace sine {
namespace tree {

template<class T>
class AVLTree : public SelfBalancedBT<T> {

public:

    AVLTree();
    AVLTree(const AVLTree<T> &);
    virtual ~AVLTree();

    virtual bool insert(const_ref);
    virtual bool remove(const_ref);

    virtual ptr find(const_ref);
    virtual const_ptr find(const_ref) const;

    virtual bool checkValid() const;
    virtual bool checkBalance() const;

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

    static bool insertToTree(const_ref, node_ptr_ref, int &sign);
    static node_ptr removeFromTree(const_ref, node_ptr_ref, int &sign);

    static ptr findInTree(const_ref, node_ptr);

    static void rotate(node_ptr_ref, bool right);
    static void fixUnbalance(node_ptr_ref, int, int &sign);  // 删除时的修复
    static node_ptr getMaxAndFix(node_ptr_ref, int &sign);

    static bool checkValidRecursive(node_ptr);
    static int debugTest(node_ptr, bool fail);
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
    int unused = 0;
    return insertToTree(t, root, unused);
}

template<class T>
bool AVLTree<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    int unused = 0;
    node_ptr del = removeFromTree(t, root, unused);
    delete del;
    return del != NULL;
}

template<class T>
typename AVLTree<T>::ptr AVLTree<T>::find(const_ref r) {
    return findInTree(r, root);
}

template<class T>
typename AVLTree<T>::const_ptr AVLTree<T>::find(const_ref r) const {
    return findInTree(r, root);
}

template<class T>
bool AVLTree<T>::checkValid() const {
    return checkValidRecursive(root);
}

template<class T>
bool AVLTree<T>::checkBalance() const {
    return testAndGetHeight(root) >= 0;
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
bool AVLTree<T>::insertToTree(const_ref v, node_ptr_ref r, int &sign) {
    if (v == r->v)
        return false;
    int a = v < r->v ? 1 : -1;
    int i = v < r->v ? 0 : 1;
    node_ptr_ref c = r->child[i];
    if (c == NULL) {
        if (r->BF == 0)
            sign = 1;
        r->BF += a;
        c = new Node(v);
        return true;
    }
    int sign2 = 0;
    if (!insertToTree(v, c, sign2))
        return false;
    if (sign2 == 0)
        return true;
    if (r->BF == 0)
        sign = 1;
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
typename AVLTree<T>::node_ptr AVLTree<T>::removeFromTree
(const_ref v, node_ptr_ref r, int &sign) {
    node_ptr rtn = r;
    int sign2 = 0;
    if (v == r->v) {  // 找到当前节点。
        if (r->child[0] != NULL) {  // 优先取左树最大值来替换。
            r = getMaxAndFix(rtn->child[0], sign2);
            r->child[0] = rtn->child[0];
            r->child[1] = rtn->child[1];
            r->BF = rtn->BF;
            if (sign2 == 1)
                fixUnbalance(r, 0, sign);
        }
        else if (r->child[1] != NULL) {  // 取右节点来替换。
            r = r->child[1];
            sign = 1;
        }
        else {
            r = NULL;
            sign = 1;
        }
        rtn->child[0] = NULL;
        rtn->child[1] = NULL;
        return rtn;
    }
    int i = v < r->v ? 0 : 1;
    node_ptr_ref c = r->child[i];
    if (c == NULL)
        return false;
    rtn = removeFromTree(v, c, sign2);
    if (rtn == NULL)
        return false;
    if (sign2 == 1)
        fixUnbalance(r, i, sign);
    return rtn;
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
void AVLTree<T>::fixUnbalance(node_ptr_ref r, int i, int &sign) {
    int a = i == 0 ? 1 : -1;
    r->BF -= a;
    if (r->BF * a < -1) {
        if (r->child[1 - i]->BF * a > 0)
            rotate(r->child[1 - i], i == 0);
        rotate(r, i == 1);
    }
    if (r->BF == 0)
        sign = 1;
}

template<class T>
typename AVLTree<T>::node_ptr AVLTree<T>::getMaxAndFix
(node_ptr_ref r, int &sign) {
    node_ptr rtn = r;
    int sign2 = 0;
    if (r->child[1] == NULL) {  // 无右节点，则自己是最大值。
        if (r->child[0] != NULL)  // 有左节点
            r = r->child[0];
        else
            r = NULL;
        sign = 1;
        return rtn;
    }
    rtn = getMaxAndFix(r->child[1], sign2);
    if (sign2 == 1)  // 子节点的高度减少了1
        fixUnbalance(r, 1, sign);
    return rtn;
}

template<class T>
typename AVLTree<T>::ptr AVLTree<T>::findInTree
(const_ref v, node_ptr root) {
    if (root == NULL)
        return NULL;
    if (v == root->v)
        return &root->v;
    int i = v < root->v ? 0 : 1;
    return findInTree(v, root->child[i]);
}

template<class T>
bool AVLTree<T>::checkValidRecursive(node_ptr r) {
    if (r != NULL) {
        if (r->child[0] != NULL)
            if (!(checkValidRecursive(r->child[0]) && r->child[0]->v < r->v))
                return false;
        if (r->child[1] != NULL)
            if (!(checkValidRecursive(r->child[1]) && r->v < r->child[1]->v))
                return false;
    }
    return true;
}

template<class T>
int AVLTree<T>::debugTest(node_ptr p, bool fail) {
    int rtn = testAndGetHeight(p);
    if (fail ^ (rtn >= 0)) {
        int unused = 0;
    }
    return rtn;
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
