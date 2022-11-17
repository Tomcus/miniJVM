#pragma once

#include <vector>

#include "jvm/types/class/base.hpp"
#include "jvm/types/const_pool.hpp"

namespace jvm {
    struct Attribute {
        Attribute() { }
        Attribute(jvm::ConstRef<std::string> attrName, std::vector<Byte> attrData): name(attrName), data(std::move(attrData)) { }
        Attribute(const Attribute& other): name(other.name), data(other.data) { }
        Attribute(Attribute && other): name(other.name), data(std::move(other.data)) { }
        Attribute& operator=(const Attribute& other) {
            name = other.name;
            data = other.data;

            return *this;
        }
        Attribute& operator=(Attribute&& other) {
            name = other.name;
            data = std::move(other.data);

            return *this;
        }
        jvm::ConstRef<std::string> name{};
        std::vector<Byte> data{};
    };

    using Attributes = std::vector<Attribute>;
}
