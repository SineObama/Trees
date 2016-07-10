#pragma once

#include <stdexcept>

namespace sine {
namespace tree {

template<class T>
class AVLTree {

public:

    typedef const T & const_ref;

    AVLTree();
    AVLTree(const AVLTree<T> &);
    ~AVLTree();

    bool insert(const_ref);
    bool remove(const_ref);

    bool test();

private:

    class Node;
    typedef Node * NodePtr;
    typedef Node *& NodePtrRef;

    struct Node {
        T v;
        NodePtr child[2];
        int BF;
        Node();
        Node(const_ref);
        NodePtrRef operator[](int index);
        static NodePtr clone(NodePtr);
        static void remove(NodePtrRef);
    };

    bool insertToNode(const_ref, NodePtrRef);
    bool removeFromNode(const_ref, NodePtrRef);

    void rotate(NodePtrRef, bool right);

    int testAndGetHeight(NodePtr);

    NodePtr root;

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
    return insertToNode(t, root);
}

template<class T>
bool AVLTree<T>::remove(const_ref t) {
    return removeFromNode(t, root);
}

template<class T>
bool AVLTree<T>::test() {
    return testAndGetHeight(root) != -1;
}

template<class T>
AVLTree<T>::Node::Node()
    : BF(0) {
    memset(child, NULL, 2 * sizeof(NodePtr));
}

template<class T>
AVLTree<T>::Node::Node(const_ref t)
    : v(t), BF(0) {
    memset(child, NULL, 2 * sizeof(NodePtr));
}

template<class T>
typename AVLTree<T>::NodePtrRef
AVLTree<T>::Node::operator[](int index) {
    if (index >= 2)
        throw std::out_of_range();
    return *child[index];
}

template<class T>
typename AVLTree<T>::NodePtr AVLTree<T>::Node::clone(NodePtr root) {
    if (root == NULL)
        return NULL;
    NodePtr rtn = new NodePtr(root->v);
    rtn->child[0] = clone(root->child[0]);
    rtn->child[1] = clone(root->child[1]);
    rtn->BF = root->BF;
    return rtn;
}

template<class T>
void AVLTree<T>::Node::remove(NodePtrRef root) {
    if (root == NULL)
        return;
    remove(root->child[0]);
    remove(root->child[1]);
    delete root;
    root = NULL;
}

template<class T>
bool AVLTree<T>::insertToNode(const_ref v, NodePtrRef r) {
    if (r == NULL)
        return r = new Node(v);
    if (v == r->v)
        return false;
    int a = v < r->v ? 1 : -1;
    int i = v < r->v ? 0 : 1;
    NodePtrRef c = r->child[i];
    int BF = c->BF;
    if (!insertToNode(v, c))
        return false;
    if (BF != 0 || c->BF == 0)
        return true;
    r->BF += a;
    if (r->BF * a > a)
        rotate(r, a);
    return true;
}

template<class T>
bool AVLTree<T>::removeFromNode(const_ref v, NodePtrRef r) {
    if (r == NULL)
        return false;
    return false;
}

template<class T>
void AVLTree<T>::rotate(NodePtrRef r, bool right) {
    int i = right ? 1 : 0;
    int a = right ? -1 : 1;
    NodePtr c = r->child[1 - i];
    r->child[1 - i] = c->child[i];
    c->child[i] = r;
    if (c->BF * a >= 0) {
        r->BF += a;
        c->BF -= a;
    }
    else {
        r->BF += - c->BF + a;
    }
    r = c;
}

template<class T>
int AVLTree<T>::testAndGetHeight(NodePtr r) {
    if (r == NULL)
        return 0;
    int h0 = testAndGetHeight(r->child[0]);
    if (h0 == -1)
        return -1;
    int h1 = testAndGetHeight(r->child[1]);
    if (h1 == -1)
        return -1;
    if (r->BF != h0 - h1)
        return -1;
    return (h0 > h1 ? h0 : h1) + 1;
}

}
}
