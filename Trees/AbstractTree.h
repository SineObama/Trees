#pragma once

#include <stdexcept>

namespace sine {
namespace tree {

template<class T>
class AbstractTree {

public:

    typedef T & ref;
    typedef const T & const_ref;
    typedef void(*handle)(const_ref);

//     virtual AbstractTree() = 0;
//     virtual AbstractTree(const AbstractTree<T> &) = 0;
    virtual ~AbstractTree() {}

    virtual bool insert(const_ref) = 0;
    virtual bool remove(const_ref) = 0;

    virtual bool find(ref) = 0;

};

}
}
