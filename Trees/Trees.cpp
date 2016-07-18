// Trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include <ctime>
#include "AVLTree.h"
#include "RBTree.h"
#include "Timer.h"

using namespace sine::tree;
using namespace std;

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

int insertNum = 100000, removeNum = 10000, findNum = 100000;

void test(BinaryTree<Container> *t) {
    Timer timer;
    timer.update();
    for (int i = 0; i < insertNum; i++) {
        t->insert(Container(rand(), 1));
    }
    cout << "insert: " << timer.update() << endl;
    int count = 0;
    timer.update();
    for (int i = 0; i < removeNum; i++) {
        if (t->remove(Container(rand(), 1)))
            count++;
    }
    cout << "remove: " << timer.update() << endl;
    timer.update();
    for (int i = 0; i < findNum; i++) {
        Container a(rand(), 0);
        t->find(a);
    }
    cout << "find: " << timer.update() << endl;
    //cout << t->test();
}

int main()
{
    long long curtime = time(NULL);
    srand(curtime & 0xFFFFFFFF);
    cout << "RBTree" << endl;
    test(&RBTree<Container>());
    cout << "AVLTree" << endl;
    test(&AVLTree<Container>());
    system("pause");
    return 0;
}
