#pragma once

#include "AbstractTree.h"

namespace sine {
namespace tree {

template<class T>
class SelfBalancedTree : public virtual AbstractTree<T> {

public:

    virtual bool checkBalance() const = 0;

};

}
}
