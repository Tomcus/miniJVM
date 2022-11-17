#pragma once

#include <optional>

#include "jvm/object/object.hpp"
#include "jvm/types/class.hpp"

namespace jvm::execution {

struct Frame {
    jvm::bytecode::Instructions& code;
    jvm::Stack& stack;
    std::vector<jvm::Object::Value> locals{};
    std::optional<jvm::Object> retValue;
    jvm::Ref<jvm::Object> thisObject;
    jvm::Ref<jvm::Class> thisClass;

};

}
