// Trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include "AVLTree.h"

using namespace sine::tree;
using namespace std;

class test {
public:
    int i, d;
    test(int ii, int dd) :i(ii), d(dd) {}
    bool operator==(const test &t) const {
        return i == t.i;
    }
    bool operator<(const test &t) const {
        return i < t.i;
    }
};

int main()
{
    AVLTree<test> t;
    for (int i = 0; i < 1000000; i++) {
        t.insert(test(rand(), 1));
    }
    /*for (int i = 0; i < 100; i++) {
        t.remove(rand());
    }*/
    test a(rand(), 0);
    cout << t.find(a);
        int re = t.test();
    //system("pause");
    return 0;
}
