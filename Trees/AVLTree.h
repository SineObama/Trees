#pragma once

#include <stdexcept>
#include "SelfBalancedBT.h"

namespace sine {
namespace tree {

template<class T>
class AVLTree : public SelfBalancedBT<T> {

public:

    virtual bool insert(const_ref);
    virtual bool remove(const_ref);

    virtual bool checkBalance() const;

private:

    class Node;
    typedef Node * node_ptr;

    class Node : public BinaryNode {
    public:
        int BF;
        Node();
        Node(const_ref);
        virtual Bnode_ptr clone();
    };

    static Bnode_ptr insertToTree(const_ref, Bnode_ptr_ref, int &sign);
    static Bnode_ptr removeFromTree(const_ref, Bnode_ptr_ref, int &sign);

    static void rotate(Bnode_ptr_ref, bool right);
    static void fixUnbalance(Bnode_ptr_ref, int, int &sign);  // ɾ��ʱ���޸�
    static Bnode_ptr pickMaxAndFix(Bnode_ptr_ref, int &sign);

    static int debugTest(node_ptr, bool fail);
    static int testAndGetHeight(node_ptr);

};

template<class T>
bool AVLTree<T>::insert(const_ref t) {
    if (root == NULL) {
        root = new Node(t);
        return true;
    }
    int unused = 0;
    return insertToTree(t, root, unused) != NULL;
}

template<class T>
bool AVLTree<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    int unused = 0;
    Bnode_ptr del = removeFromTree(t, root, unused);
    delete del;
    return del != NULL;
}

template<class T>
bool AVLTree<T>::checkBalance() const {
    return testAndGetHeight(dynamic_cast<node_ptr>(root)) >= 0;
}

template<class T>
AVLTree<T>::Node::Node()
    : BF(0) {
}

template<class T>
AVLTree<T>::Node::Node(const_ref v)
    : BinaryNode(v), BF(0) {
}

template<class T>
typename AVLTree<T>::Bnode_ptr AVLTree<T>::Node::clone() {
    node_ptr rtn = new Node(v);
    rtn->BF = BF;
    if (child[0] != NULL)
        rtn->child[0] = child[0]->clone();
    if (child[1] != NULL)
        rtn->child[1] = child[1]->clone();
    return rtn;
}

/**
 * �����ܿսڵ�
 */
template<class T>
typename AVLTree<T>::Bnode_ptr AVLTree<T>::insertToTree
(const_ref v, Bnode_ptr_ref _r, int &sign) {
    if (v == _r->v)
        return NULL;
    int a = v < _r->v ? 1 : -1;
    int i = v < _r->v ? 0 : 1;
    Bnode_ptr_ref _c = _r->child[i];
    node_ptr r = dynamic_cast<node_ptr>(_r);
    if (_c == NULL) {
        if (r->BF == 0)
            sign = 1;
        r->BF += a;
        _c = new Node(v);
        return _c;
    }
    int sign2 = 0;
    Bnode_ptr p = insertToTree(v, _c, sign2);
    if (p == NULL)
        return NULL;
    if (sign2 == 0)
        return p;
    if (r->BF == 0)
        sign = 1;
    r->BF += a;
    if (r->BF * a > 1) {
        if (dynamic_cast<node_ptr>(_c)->BF * a < 0)
            rotate(_c, i == 1);
        rotate(_r, i == 0);
    }
    return p;
}

/**
* �����ܿսڵ�
*/
template<class T>
typename AVLTree<T>::Bnode_ptr AVLTree<T>::removeFromTree
(const_ref v, Bnode_ptr_ref _r, int &sign) {
    Bnode_ptr rtn = _r;
    int sign2 = 0;
    if (v == _r->v) {  // �ҵ���ǰ�ڵ㡣
        if (_r->child[0] != NULL) {  // ����ȡ�������ֵ���滻��
            _r = pickMaxAndFix(rtn->child[0], sign2);
            _r->child[0] = rtn->child[0];
            _r->child[1] = rtn->child[1];
            dynamic_cast<node_ptr>(_r)->BF = 
                dynamic_cast<node_ptr>(rtn)->BF;
            if (sign2 == 1)
                fixUnbalance(_r, 0, sign);
        }
        else if (_r->child[1] != NULL) {  // ȡ�ҽڵ����滻��
            _r = _r->child[1];
            sign = 1;
        }
        else {
            _r = NULL;
            sign = 1;
        }
        rtn->child[0] = NULL;
        rtn->child[1] = NULL;
        return rtn;
    }
    int i = v < _r->v ? 0 : 1;
    Bnode_ptr_ref _c = _r->child[i];
    if (_c == NULL)
        return false;
    rtn = removeFromTree(v, _c, sign2);
    if (rtn == NULL)
        return false;
    if (sign2 == 1)
        fixUnbalance(_r, i, sign);
    return rtn;
}

/**
 * ���㣺
 * ����������
 * ������������
 * ������������
 * ��������
 * ����������
 * x,y,z�ǽڵ���ȣ�m,n��ƽ�����ӡ�׼��������
 * m = x - y, n = Max{x, y} + 1 - z
 * ��������m2
 * ������������
 * ������������n2
 * ��������������\
 * ������������������
 * m2 = x - Max{y, z} - 1, n2 = y - z
 * ��2�������
 * 1. x <= y �� m <= 0
 * m = x - y, n = y - z + 1
 * m2 = Min{m, m + n - 1} - 1, n2 = n - 1
 * 2. x > y �� m > 0
 * m = x - y, n = x - z + 1;
 * m2 = Min{m, n - 1} - 1, n2 = n - m - 1;
 * �ۺϣ�
 * m2 - m = Min{0, n2} - 1, n2 - n = Min{0, -m} - 1
 */
template<class T>
void AVLTree<T>::rotate(Bnode_ptr_ref _r, bool right) {
    int i = right ? 1 : 0;
    int a = right ? -1 : 1;
    Bnode_ptr _c = _r->child[1 - i];
    _r->child[1 - i] = _c->child[i];
    _c->child[i] = _r;
    int cBF = dynamic_cast<node_ptr>(_c)->BF;
    int rBF = 
        (dynamic_cast<node_ptr>(_r)->BF += a - (a * cBF < 0 ? cBF : 0));
    dynamic_cast<node_ptr>(_c)->BF += a + (a * rBF > 0 ? rBF : 0);
    _r = _c;
}

template<class T>
void AVLTree<T>::fixUnbalance(Bnode_ptr_ref _r, int i, int &sign) {
    int a = i == 0 ? 1 : -1;
    dynamic_cast<node_ptr>(_r)->BF -= a;
    if (dynamic_cast<node_ptr>(_r)->BF * a < -1) {
        if (dynamic_cast<node_ptr>(_r->child[1 - i])->BF * a > 0)
            rotate(_r->child[1 - i], i == 0);
        rotate(_r, i == 1);
    }
    if (dynamic_cast<node_ptr>(_r)->BF == 0)
        sign = 1;
}

template<class T>
typename AVLTree<T>::Bnode_ptr AVLTree<T>::pickMaxAndFix
(Bnode_ptr_ref _r, int &sign) {
    Bnode_ptr rtn = _r;
    int sign2 = 0;
    if (_r->child[1] == NULL) {  // ���ҽڵ㣬���Լ������ֵ��
        if (_r->child[0] != NULL)  // ����ڵ�
            _r = _r->child[0];
        else
            _r = NULL;
        sign = 1;
        return rtn;
    }
    rtn = pickMaxAndFix(_r->child[1], sign2);
    if (sign2 == 1)  // �ӽڵ�ĸ߶ȼ�����1
        fixUnbalance(_r, 1, sign);
    return rtn;
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
    int h0 = testAndGetHeight(dynamic_cast<node_ptr>(r->child[0]));
    if (h0 == -1)
        return -1;
    int h1 = testAndGetHeight(dynamic_cast<node_ptr>(r->child[1]));
    if (h1 == -1)
        return -1;
    if (r->BF * r->BF > 1 || r->BF != h0 - h1)
        return -1;
    return (h0 > h1 ? h0 : h1) + 1;
}

}
}
