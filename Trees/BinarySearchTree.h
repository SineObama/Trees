#pragma once

#include "BinaryTree.h"
#include "SearchTree.h"

namespace sine {
namespace tree {

template<class T>
class BinarySearchTree
    : public virtual BinaryTree<T>, public virtual SearchTree<T> {
};

}
}
