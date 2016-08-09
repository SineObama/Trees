#pragma once

#include "BinaryTree.h"

namespace sine {
namespace tree {

template<class T>
class SelfBalancedBT : public BinaryTree<T> {

public:

    virtual bool check() = 0;

};

}
}
