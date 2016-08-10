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

    class Node : public BinaryNode {
    public:
        bool red;
        Node();
        Node(const_ref);
        virtual Bnode_ptr clone();
    };

    static bool insertToTree(const_ref, Bnode_ptr_ref, int &sign);
    static Bnode_ptr removeFromTree(const_ref, Bnode_ptr_ref, int &sign);

    static ptr findInTree(const_ref, Bnode_ptr);

    static void rotate(Bnode_ptr_ref, bool right);
    static void fixUnbalance(Bnode_ptr_ref, int, int &sign);  // ɾ��ʱ���޸�
    static Bnode_ptr getMaxAndFix(Bnode_ptr_ref, int &sign);

    static void fixRedBlack(Bnode_ptr_ref, int);  // ɾ��ʱ��һ�����

    static bool checkValidRecursive(Bnode_ptr);
    static int debugTest(node_ptr, bool fail);
    static int testAndGetBlacks(node_ptr);

};

#define IS_RED(r) (r != NULL && r->red)

template<class T>
RBTree<T>::RBTree() {
    root = NULL;
}

template<class T>
RBTree<T>::RBTree(const RBTree<T> &o) {
    if (o.root != NULL)
        root = o.root->clone();
}

template<class T>
RBTree<T>::~RBTree() {
    Node::remove(root);
}

template<class T>
bool RBTree<T>::insert(const_ref t) {
    if (root == NULL) {
        node_ptr newRoot = new Node(t);
        newRoot->red = false;
        root = newRoot;
        return true;
    }
    int unused;
    if (!insertToTree(t, root, unused))
        return false;
    dynamic_cast<node_ptr>(root)->red = false;
    return true;
}

template<class T>
bool RBTree<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    int unused;
    Bnode_ptr p = removeFromTree(t, root, unused);
    if (root != NULL)
        dynamic_cast<node_ptr>(root)->red = false;
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
    return testAndGetBlacks(dynamic_cast<node_ptr>(root)) >= 0;
}

template<class T>
RBTree<T>::Node::Node()
    : red(true) {
}

template<class T>
RBTree<T>::Node::Node(const_ref v)
    : BinaryNode(v), red(true) {
}

template<class T>
typename RBTree<T>::Bnode_ptr RBTree<T>::Node::clone() {
    node_ptr rtn = new Node(v);
    rtn->red = red;
    if (child[0] != NULL)
        rtn->child[0] = child[0]->clone();
    if (child[1] != NULL)
        rtn->child[1] = child[1]->clone();
    return rtn;
}

/**
 * �����ܿ�ָ�롣
 * �ź�-1��ʾ�ޱ仯��0��1��ʾ����ҽڵ���ֳ�ͻ���͵�ǰ�ڵ�ͬΪ��ɫ��
 */
template<class T>
bool RBTree<T>::insertToTree(const_ref v, Bnode_ptr_ref _r, int &sign) {
    if (v == _r->v)
        return false;
    int i = v < _r->v ? 0 : 1;
    Bnode_ptr_ref _c = _r->child[i];
    if (_c == NULL) {
        _c = new Node(v);
        if (dynamic_cast<node_ptr>(_r)->red)
            sign = i;
        return true;
    }
    int sign2 = -1;
    if (!insertToTree(v, _c, sign2))
        return false;
    if (sign2 != -1) {
        if (sign2 != i)
            rotate(_c, i == 1);
        dynamic_cast<node_ptr>(_c->child[i])->red = false;
        rotate(_r, i == 0);
    }
    else if (dynamic_cast<node_ptr>(_r)->red && dynamic_cast<node_ptr>(_c)->red)
        sign = i;
    return true;
}

/**
 * �����ܿ�ָ�롣
 * �ź�1��ʾ�ڽڵ�������1���ź�0��ʾ�ޱ仯��
 */
template<class T>
typename RBTree<T>::Bnode_ptr RBTree<T>::removeFromTree
(const_ref v, Bnode_ptr_ref _r, int &sign) {
    Bnode_ptr rtn = _r;
    int sign2 = 0;
    if (v == _r->v) {  // �ҵ���ǰ�ڵ㡣
        if (_r->child[0] != NULL) {  // ����ȡ�������ֵ���滻��
            _r = getMaxAndFix(rtn->child[0], sign2);
            _r->child[0] = rtn->child[0];
            _r->child[1] = rtn->child[1];
            dynamic_cast<node_ptr>(_r)->red = dynamic_cast<node_ptr>(rtn)->red;
            if (sign2 == 1)
                fixUnbalance(_r, 0, sign);
        }
        else if (_r->child[1] != NULL) {  // ȡ�ҽڵ����滻��
            _r = _r->child[1];
            dynamic_cast<node_ptr>(_r)->red = false;
        }
        else {
            if (!dynamic_cast<node_ptr>(_r)->red)
                sign = 1;
            _r = NULL;
        }
        rtn->child[0] = NULL;
        rtn->child[1] = NULL;
        return rtn;
    }
    // ���¼�������
    int i = v < _r->v ? 0 : 1;  // ��̽����
    Bnode_ptr_ref _c = _r->child[i];  // ��̽�ڵ�
    if (_c == NULL)
        return NULL;
    rtn = removeFromTree(v, _c, sign2);
    if (rtn == NULL)
        return NULL;
    if (sign2 == 1)  // �ӽڵ�ĺڽڵ���������1
        fixUnbalance(_r, i, sign);
    return rtn;
}

template<class T>
void RBTree<T>::rotate(Bnode_ptr_ref _r, bool right) {
    int i = right ? 1 : 0;
    Bnode_ptr _c = _r->child[1 - i];
    _r->child[1 - i] = _c->child[i];
    _c->child[i] = _r;
    _r = _c;
}

// �޸�i�����Ϻڽڵ�������1�����µĲ�ƽ�⡣
template<class T>
void RBTree<T>::fixUnbalance(Bnode_ptr_ref _r, int i, int &sign) {
    // ����ʱĬ��iΪ1
    Bnode_ptr_ref _other = _r->child[1 - i];
    node_ptr r = dynamic_cast<node_ptr>(_r);
    node_ptr other = dynamic_cast<node_ptr>(_other);
    if (!r->red && !other->red) {  // �� /��\ �ڡ�������Ԥ����
        node_ptr a = dynamic_cast<node_ptr>(_other->child[1 - i]);
        node_ptr b = dynamic_cast<node_ptr>(_other->child[i]);
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
            rotate(_other, i == 0);
        }
    }
    other = dynamic_cast<node_ptr>(_other);
    if (r->red) {  // �� /��\ ��
        fixRedBlack(_r, i);
    }
    else if (other->red) {  // �� /��\ ��
        other->red = false;
        r->red = true;
        rotate(_r, i == 1);
        // �� /��\ ��/��\��
        fixRedBlack(_r->child[i], i);
    }
    else {  // ��/��\�� /��\ ��
        dynamic_cast<node_ptr>(_other->child[1 - i])->red = false;
        rotate(_r, i == 1);
    }
}

template<class T>
typename RBTree<T>::Bnode_ptr RBTree<T>::getMaxAndFix(Bnode_ptr_ref _r, int &sign) {
    Bnode_ptr rtn = _r;
    int sign2 = 0;
    if (_r->child[1] == NULL) {  // ���ҽڵ㣬���Լ������ֵ��
        if (_r->child[0] != NULL) {  // ����ڵ㣨��Ϊ��ɫ��
            _r = _r->child[0];
            dynamic_cast<node_ptr>(_r)->red = false;
        }
        else {
            if (!dynamic_cast<node_ptr>(_r)->red)
                sign = 1;
            _r = NULL;
        }
        return rtn;
    }
    rtn = getMaxAndFix(_r->child[1], sign2);
    if (sign2 == 1)  // �ӽڵ�ĺڽڵ���������1
        fixUnbalance(_r, 1, sign);
    return rtn;
}

// ɾ��ʱ��ƽ����������ڵ�Ϊ������
template<class T>
void RBTree<T>::fixRedBlack(Bnode_ptr_ref _r, int i) {
    Bnode_ptr_ref _other = _r->child[1 - i];
    node_ptr a = dynamic_cast<node_ptr>(_other->child[1 - i]);
    node_ptr b = dynamic_cast<node_ptr>(_other->child[i]);
    if (IS_RED(b)) {
        if (IS_RED(a)) {
            a->red = false;
            dynamic_cast<node_ptr>(_other)->red = true;
            dynamic_cast<node_ptr>(_r)->red = false;
        }
        else {
            dynamic_cast<node_ptr>(_other)->red = true;
            b->red = false;
            rotate(_other, i == 0);
        }
    }
    rotate(_r, i == 1);
}

template<class T>
typename RBTree<T>::ptr RBTree<T>::findInTree(const_ref v, Bnode_ptr root) {
    if (root == NULL)
        return NULL;
    if (v == root->v)
        return &root->v;
    int i = v < root->v ? 0 : 1;
    return findInTree(v, root->child[i]);
}

template<class T>
bool RBTree<T>::checkValidRecursive(Bnode_ptr _r) {
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
    node_ptr c0 = dynamic_cast<node_ptr>(r->child[0]);
    node_ptr c1 = dynamic_cast<node_ptr>(r->child[1]);
    if (IS_RED(r) && (IS_RED(c0) || IS_RED(c1)))
        return -(1 << 30);

    int b0 = testAndGetBlacks(c0);
    if (b0 < 0)
        return b0;
    if (b0 > 0 && !(c0->v < r->v))
        return -(1 << 29);

    int b1 = testAndGetBlacks(c1);
    if (b1 < 0)
        return b1;
    if (b1 > 0 && !(r->v < c1->v))
        return -(1 << 29);

    if (b0 != b1)
        return b1 > b0 ? b0 - b1 : b1 - b0;
    return b0 + (r->red ? 0 : 1);
}

#undef IS_RED

}
}
