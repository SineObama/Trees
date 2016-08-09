#pragma once

#include <stdexcept>
#include <cassert>
#include "SelfBalancedBT.h"

namespace sine {
namespace tree {

template<class T>
class RBTree : public SelfBalancedBT<T> {

public:

    RBTree();
    RBTree(const RBTree<T> &);
    virtual ~RBTree();

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
        bool red;
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
    static void fixUnbalance(node_ptr_ref, int, int &sign);  // ɾ��ʱ���޸�
    static node_ptr getMaxAndFix(node_ptr_ref, int &sign);
    static void fixRedBlack(node_ptr_ref, int);  // ɾ��ʱ��һ�����

    static bool checkValidRecursive(node_ptr);
    static int debugTest(node_ptr, bool fail);
    static int testAndGetBlacks(node_ptr);

    node_ptr root;

};

#define IS_RED(r) (r != NULL && r->red)

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
    int unused;
    node_ptr p = removeFromTree(t, root, unused);
    if (root != NULL)
        root->red = false;
    delete p;
    return p != NULL;
}

template<class T>
typename RBTree<T>::ptr RBTree<T>::find(const_ref r) {
    return findInTree(r, root);
}

template<class T>
typename RBTree<T>::const_ptr RBTree<T>::find(const_ref r) const {
    return findInTree(r, root);
}

template<class T>
bool RBTree<T>::checkValid() const {
    return checkValidRecursive(root);
}

template<class T>
bool RBTree<T>::checkBalance() const {
    return testAndGetBlacks(root) >= 0;
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
    node_ptr rtn = new Node(root->v);
    rtn->child[0] = clone(root->child[0]);
    rtn->child[1] = clone(root->child[1]);
    rtn->red = root->red;
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

/**
 * �����ܿ�ָ�롣
 * �ź�-1��ʾ�ޱ仯��0��1��ʾ����ҽڵ���ֳ�ͻ���͵�ǰ�ڵ�ͬΪ��ɫ��
 */
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

/**
 * �����ܿ�ָ�롣
 * �ź�1��ʾ�ڽڵ�������1���ź�0��ʾ�ޱ仯��
 */
template<class T>
typename RBTree<T>::node_ptr RBTree<T>::removeFromTree
(const_ref v, node_ptr_ref r, int &sign) {
    node_ptr rtn = r;
            int sign2 = 0;
    if (v == r->v) {  // �ҵ���ǰ�ڵ㡣
        if (r->child[0] != NULL) {  // ����ȡ�������ֵ���滻��
            r = getMaxAndFix(rtn->child[0], sign2);
            r->child[0] = rtn->child[0];
            r->child[1] = rtn->child[1];
            r->red = rtn->red;
            if (sign2)
                fixUnbalance(r, 0, sign);
        }
        else if (r->child[1] != NULL) {  // ȡ�ҽڵ����滻��
            r = r->child[1];
            r->red = false;
        }
        else {
            if (!r->red)
                sign = 1;
            r = NULL;
        }
        rtn->child[0] = NULL;
        rtn->child[1] = NULL;
        return rtn;
    }
    // ���¼�������
    int i = v < r->v ? 0 : 1;  // ��̽����
    node_ptr_ref c = r->child[i];  // ��̽�ڵ�
    if (c == NULL)
        return NULL;
    rtn = removeFromTree(v, c, sign2);
    if (!rtn)
        return NULL;
    if (sign2 == 1)  // �ӽڵ�ĺڽڵ���������1
        fixUnbalance(r, i, sign);
    return rtn;
}

template<class T>
void RBTree<T>::rotate(node_ptr_ref r, bool right) {
    int i = right ? 1 : 0;
    node_ptr c = r->child[1 - i];
    r->child[1 - i] = c->child[i];
    c->child[i] = r;
    r = c;
}

// �޸�i�����Ϻڽڵ�������1�����µĲ�ƽ�⡣
template<class T>
void RBTree<T>::fixUnbalance(node_ptr_ref r, int i, int &sign) {
    // ����ʱĬ��iΪ1
    node_ptr_ref other = r->child[1 - i];
    if (!r->red && !other->red) {  // �� /��\ �ڡ�������Ԥ����
        node_ptr a = other->child[1 - i], b = other->child[i];
        bool ared = IS_RED(a), bred = IS_RED(b);
        if (!ared && !bred) {  // ��/��\�� /��\ ��
            other->red = true;
            sign = 1;
            return;
        }
        else if (ared && bred) {  // ��/��\�� /��\ �� => �� /��\ ��
            other->red = true;
            a->red = false;
            b->red = false;
        }
        else if (bred) {  // ��/��\�� /��\ �� => ��/��\�� /��\ ��
            other->red = true;
            b->red = false;
            rotate(other, i == 0);
        }
    }
    if (r->red) {  // �� /��\ ��
        fixRedBlack(r, i);
    }
    else if (other->red) {  // �� /��\ ��
        other->red = false;
        r->red = true;
        rotate(r, i == 1);
        // �� /��\ ��/��\��
        fixRedBlack(r->child[i], i);
    }
    else {  // ��/��\�� /��\ ��
        other->child[1 - i]->red = false;
        rotate(r, i == 1);
    }
}

template<class T>
typename RBTree<T>::node_ptr RBTree<T>::getMaxAndFix(node_ptr_ref r, int &sign) {
    node_ptr rtn = r;
    int sign2 = 0;
    if (r->child[1] == NULL) {  // ���ҽڵ㣬���Լ������ֵ��
        if (r->child[0] != NULL) {  // ����ڵ㣨��Ϊ��ɫ��
            r = r->child[0];
            r->red = false;
        }
        else {
            if (!r->red)
                sign = 1;
            r = NULL;
        }
        return rtn;
    }
    rtn = getMaxAndFix(r->child[1], sign2);
    if (sign2 == 1)  // �ӽڵ�ĺڽڵ���������1
        fixUnbalance(r, 1, sign);
    return rtn;
}

// ɾ��ʱ��ƽ����������ڵ�Ϊ������
template<class T>
void RBTree<T>::fixRedBlack(node_ptr_ref r, int i) {
    node_ptr_ref other = r->child[1 - i];
    node_ptr a = other->child[1 - i], b = other->child[i];
    if (IS_RED(b)) {
        if (IS_RED(a)) {
            a->red = false;
            other->red = true;
            r->red = false;
        }
        else {
            other->red = true;
            b->red = false;
            rotate(other, i == 0);
        }
    }
    rotate(r, i == 1);
}

template<class T>
typename RBTree<T>::ptr RBTree<T>::findInTree(const_ref v, node_ptr root) {
    if (root == NULL)
        return NULL;
    if (v == root->v)
        return &root->v;
    int i = v < root->v ? 0 : 1;
    return findInTree(v, root->child[i]);
}

template<class T>
bool RBTree<T>::checkValidRecursive(node_ptr r) {
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
int RBTree<T>::debugTest(node_ptr p, bool fail) {
    int rtn = testAndGetBlacks(p);
    if (fail ^ (rtn >= 0)) {
        int unused = 0;
    }
    return rtn;
}

template<class T>
int RBTree<T>::testAndGetBlacks(node_ptr r) {
    if (r == NULL)
        return 0;
    if (IS_RED(r) && (IS_RED(r->child[0]) || IS_RED(r->child[1])))
        return -(1 << 30);

    int b0 = testAndGetBlacks(r->child[0]);
    if (b0 < 0)
        return b0;
    if (b0 > 0 && !(r->child[0]->v < r->v))
        return -(1 << 29);

    int b1 = testAndGetBlacks(r->child[1]);
    if (b1 < 0)
        return b1;
    if (b1 > 0 && !(r->v < r->child[1]->v))
        return -(1 << 29);

    if (b0 != b1)
        return b1 > b0 ? b0 - b1 : b1 - b0;
    return b0 + (r->red ? 0 : 1);
}

#undef IS_RED

}
}
