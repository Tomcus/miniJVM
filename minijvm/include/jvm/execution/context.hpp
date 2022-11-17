#pragma once

#include "jvm/execution/stack.hpp"
#include "jvm/object/object.hpp"
#include "jvm/types/class.hpp"

#include <string_view>
#include <unordered_map>
#include <string>

namespace jvm {

class ExecutionContext{
public:
    ExecutionContext(std::string_view classLocation, std::string_view methodName);
    void run(std::vector<std::string> args = {});

    jvm::Ref<jvm::Object> createObjectInstance(const jvm::Class& definition);
protected:
    std::unordered_map<std::string, jvm::Class> classes;
    std::vector<jvm::Object> gc; // Yeah totally :D
    Stack stack;
};

}
