// Trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include "AVLTree.h"

using namespace sine::tree;
using namespace std;

int main()
{
    AVLTree<int> t;
    for (int i = 0; i < 100000; i++) {
        t.insert(rand());
    }
    for (int i = 0; i < 100000; i++) {
        t.remove(rand());
    }
        int re = t.test();
    //system("pause");
    return 0;
}
