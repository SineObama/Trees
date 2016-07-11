// Trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include <ctime>
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
    srand(time(NULL));
    AVLTree<test> t;
    for (int i = 0; i < 100000; i++) {
        t.insert(test(rand(), 1));
    }
    for (int i = 0; i < 10000; i++) {
        t.remove(test(rand(), 1));
    }
    for (int i = 0; i < 100; i++) {
        test a(rand(), 0);
        if (t.find(a))
            cout << a.d << endl;
    }
    int re = t.test();
    cout << re << endl;
    system("pause");
    return 0;
}
