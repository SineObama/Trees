#pragma once

#include "BinarySearchTree.h"

namespace sine {
namespace tree {

template<class T>
class SelfBalancedBT : public virtual BinarySearchTree<T> {

public:

    virtual bool checkBalance() const = 0;

};

}
}
