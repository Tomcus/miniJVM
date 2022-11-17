#pragma once

#include <cstdint>
#include <variant>
#include <jvm/utils/ref.hpp>
#include <jvm/types/class.hpp>
#include <vector>

namespace jvm {

class Object {
public:
    using Value = std::variant<std::monostate, int, long, float, double, jvm::Ref<Object>, std::vector<jvm::Ref<Object>>>;

    explicit Object(jvm::ConstRef<jvm::Class> definition);

    template<typename Type>
    Type& get(std::uint16_t index) {
        return std::move(std::get<Type>(fields.at(index)));
    }
 
    template<typename Type>
    const Type& get(std::uint16_t index) const {
        return std::move(std::get<Type>(fields.at(index)));
    }

    template<typename Type>
    void set(std::uint16_t index, Type value) {
        auto& val = fields.at(index);
        if (std::holds_alternative<Type>(val)) {
            val = std::move(value);
        }
    }
protected:
    jvm::ConstRef<jvm::Class> self;
    std::vector<Value> fields;
};

}
