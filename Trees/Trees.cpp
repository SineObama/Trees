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

int insertNum = 100000, removeNum = 100000, findNum = 100000;

class Container;
void test(SelfBalancedBT<Container> *t);
int random(int bit = 18);

int main()
{
    long long curtime = time(NULL);
    //curtime = 1470814693;
    srand(curtime & 0xFFFFFFFF);
    cout << "RBTree" << endl;
    test(&RBTree<Container>());
    cout << "AVLTree" << endl;
    test(&AVLTree<Container>());
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

void test(SelfBalancedBT<Container> *t) {
    Timer timer;
    timer.update();
    for (int i = 0; i < insertNum; i++) {
        t->insert(Container(random(), 1));
    }
    cout << "insert: " << timer.update() << endl;
    cout << "checkValid: " << t->checkValid() << endl;
    cout << "checkBalance: " << t->checkBalance() << endl;
    int count = 0;
    timer.update();
    for (int i = 0; i < removeNum; i++) {
        if (t->remove(Container(random(), 1)))
            count++;
    }
    cout << "remove: " << timer.update() << endl;
    cout << "checkValid: " << t->checkValid() << endl;
    cout << "checkBalance: " << t->checkBalance() << endl;
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
