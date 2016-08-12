#pragma once

#include "BinarySearchTree.h"
#include "SelfBalancedTree.h"

namespace sine {
namespace tree {

template<class T>
class SelfBalancedBT
    : public virtual BinarySearchTree<T>,
    public virtual SelfBalancedTree<T> {
};

}
}
