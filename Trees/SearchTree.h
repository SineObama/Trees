#pragma once

#include "AbstractTree.h"

namespace sine {
namespace tree {

template<class T>
class SearchTree : public virtual AbstractTree<T> {

public:

    virtual bool insert(const_ref) = 0;
    virtual bool remove(const_ref) = 0;

    virtual ptr find(const_ref) = 0;
    virtual const_ptr find(const_ref) const = 0;

    virtual bool checkValid() const = 0;  // 检查正确性（比对大小）

};

}
}
