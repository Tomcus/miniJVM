#pragma once

#include "jvm/types/class.hpp"
#include "jvm/types/basic.hpp"
#include "jvm/utils/ref.hpp"

#include <variant>
#include <string>
#include <unordered_map>

namespace jvm {

class ClassInstanceBase: public ClassBase {
public:
protected:
    ConstRef<ClassBase> classRef;
};

class ClassInstance: public ClassInstanceBase {
public:
    
protected:
    std::unordered_map<std::string, VariableType> variables;
};
}
