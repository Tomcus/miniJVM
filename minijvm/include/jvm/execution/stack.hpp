#pragma once

#include <vector>

#include <jvm/object/object.hpp>

namespace jvm {

class Stack {
public:
    using Value = jvm::Object::Value;
    Stack();

    void push(Value value);
    Value pop();
protected:
    std::vector<Value> data;
};

}
