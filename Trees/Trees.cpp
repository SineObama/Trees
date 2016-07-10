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
    t.insert(1);
    cout << t.test();
    system("pause");
    return 0;
}
