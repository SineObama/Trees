#pragma once

namespace sine {
namespace tree {

template<class T>
class AbstractTree {

public:

    typedef T * ptr;
    typedef const T * const_ptr;
    typedef T & ref;
    typedef const T & const_ref;
    typedef void(*handle)(const_ref);

    virtual ~AbstractTree() {}

};

}
}
