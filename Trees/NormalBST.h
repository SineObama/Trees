#pragma once

#include "BinarySearchTree.h"

namespace sine {
namespace tree {

/**
 * ��ͨ���������
 */
template<class T>
class NormalBST : public virtual BinarySearchTree<T> {

public:

    virtual bool insert(const_ref);
    virtual bool remove(const_ref);

private:

    static Bnode_ptr insertToTree(const_ref, Bnode_ptr);
    static Bnode_ptr removeFromTree(const_ref, Bnode_ptr_ref);

    static Bnode_ptr pickMax(Bnode_ptr_ref);
    static Bnode_ptr pickMin(Bnode_ptr_ref);

};

template<class T>
bool NormalBST<T>::insert(const_ref t) {
    if (root == NULL) {
        root = new BinaryNode(t);
        return true;
    }
    return insertToTree(t, root) != NULL;
}

template<class T>
bool NormalBST<T>::remove(const_ref t) {
    if (root == NULL)
        return false;
    Bnode_ptr del = removeFromTree(t, root);
    delete del;
    return del != NULL;
}

/**
* �����ܿսڵ�
*/
template<class T>
typename NormalBST<T>::Bnode_ptr NormalBST<T>::insertToTree
(const_ref v, Bnode_ptr _r) {
    if (v == _r->v)
        return NULL;
    Bnode_ptr_ref _c = _r->child[v < _r->v ? 0 : 1];
    if (_c != NULL)
        return insertToTree(v, _c);
    _c = new BinaryNode(v);
    return _c;
}

/**
* �����ܿսڵ�
*/
template<class T>
typename NormalBST<T>::Bnode_ptr NormalBST<T>::removeFromTree
(const_ref v, Bnode_ptr_ref _r) {
    Bnode_ptr rtn = _r;
    if (v == _r->v) {  // �ҵ���ǰ�ڵ㡣
        if (_r->child[0] != NULL) {  // ����ȡ�������ֵ���滻��
            _r = pickMax(rtn->child[0]);
            _r->child[0] = rtn->child[0];
            _r->child[1] = rtn->child[1];
        }
        else if (_r->child[1] != NULL) {  // ȡ������Сֵ���滻��
            _r = pickMin(rtn->child[1]);
            _r->child[1] = rtn->child[1];
        }
        else {
            _r = NULL;
        }
        rtn->child[0] = NULL;
        rtn->child[1] = NULL;
        return rtn;
    }
    Bnode_ptr_ref _c = _r->child[v < _r->v ? 0 : 1];
    if (_c == NULL)
        return NULL;
    return removeFromTree(v, _c);
}

/**
* �����ܿսڵ�
*/
template<class T>
typename NormalBST<T>::Bnode_ptr NormalBST<T>::pickMax(Bnode_ptr_ref _r) {
    if (_r->child[1] != NULL) 
        return pickMax(_r->child[1]);
    Bnode_ptr rtn = _r;
    if (_r->child[0] != NULL) {
        _r = pickMax(rtn->child[0]);
        _r->child[0] = rtn->child[0];
    }
    else {
        _r = NULL;
    }
    return rtn;
}

/**
* �����ܿսڵ�
*/
template<class T>
typename NormalBST<T>::Bnode_ptr NormalBST<T>::pickMin(Bnode_ptr_ref _r) {
    if (_r->child[0] != NULL)
        return pickMin(_r->child[0]);
    Bnode_ptr rtn = _r;
    if (_r->child[1] != NULL) {
        _r = pickMin(rtn->child[1]);
        _r->child[1] = rtn->child[1];
    }
    else {
        _r = NULL;
    }
    return rtn;
}

}
}
