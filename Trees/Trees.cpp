// Trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include <ctime>
#include "NormalBST.h"
#include "AVLTree.h"
#include "RBTree.h"
#include "Timer.h"

using namespace sine::tree;
using namespace std;

int insertNum = 100000, removeNum = 100000, findNum = 100000;

class Container;
void handler(Container &);
void const_handler(const Container &c);
void test(BinarySearchTree<Container> *t);
int random(int bit = 18);

int main()
{
    long long curtime = time(NULL);

    {
        srand(curtime & 0xFFFFFFFF);
        cout << "RBTree" << endl;
        RBTree<Container> a;
        test(&a);
        RBTree<Container> b(a);
    }
    
    {
        srand(curtime & 0xFFFFFFFF);
        cout << "AVLTree" << endl;
        AVLTree<Container> a;
        test(&a);
        AVLTree<Container> b(a);
    }

    {
        srand(curtime & 0xFFFFFFFF);
        cout << "NormalBST" << endl;
        NormalBST<Container> a;
        test(&a);
        NormalBST<Container> b(a);
    }

    system("pause");
    return 0;
}

class Container {
public:
    int i, d;
    Container(int ii, int dd) :i(ii), d(dd) {}
    bool operator==(const Container &t) const {
        return i == t.i;
    }
    bool operator<(const Container &t) const {
        return i < t.i;
    }
};

void handler(Container &c) {
    cout << c.i << " ";
}

void const_handler(const Container &c) {
    static int i = 0;
    cout << i++ << " " << c.i << endl;
}

void test(BinarySearchTree<Container> *t) {
    Timer timer;
    timer.update();
    for (int i = 0; i < insertNum; i++) {
        t->insert(Container(random(), 1));
    }
    cout << "insert: " << timer.update() << endl;
    cout << "checkValid: " << t->checkValid() << endl;

    int count = 0;
    timer.update();
    for (int i = 0; i < removeNum; i++) {
        if (t->remove(Container(random(), 1)))
            count++;
    }
    cout << "remove: " << timer.update() << endl;
    cout << "checkValid: " << t->checkValid() << endl;

    //const SelfBalancedBT<Container> *tem = t;
    //tem->traverse(const_handler, inOrder);
    //cout << endl;

    timer.update();
    for (int i = 0; i < findNum; i++) {
        Container a(random(), 0);
        t->find(a);
    }
    cout << "find: " << timer.update() << endl;
}

int random(int bit) {
    static const int s = (1 << 15) - 1;
    if (bit < 16)
        return rand() & (s >> (15 - bit));
    if (bit < 31)
        return (rand() & s) | ((rand() & (s >> (30 - bit))) << 15);
    throw;
}
