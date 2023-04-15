#include "jvm/types/class/class_field.hpp"

#include <algorithm>

using namespace jvm;

std::vector<std::uint8_t> ClassField::getAndRemoveAttributeData(std::string_view attributeName) {
    std::vector<std::uint8_t> res;
    auto it = std::ranges::find_if(attributes, [&](auto& attribute) {
        return std::string(attribute.name) == attributeName;
    });

    // FIXME: Return error when not found.

    res = std::move(it->data);

    attributes.erase(it);

    return res;
}
