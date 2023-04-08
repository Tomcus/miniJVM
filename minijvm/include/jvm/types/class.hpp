#ifndef MINI_JVM_TYPES_CLASS_HPP
#define MINI_JVM_TYPES_CLASS_HPP

#include "jvm/types/basic.hpp"
#include "jvm/types/const_pool.hpp"
#include "jvm/types/class/attribute.hpp"
#include "jvm/types/class/field.hpp"
#include "jvm/types/class/method.hpp"
#include "jvm/utils/ref.hpp"

#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <filesystem>

namespace jvm {

struct ver16 {
    std::uint16_t major;
    std::uint16_t minor;
};

using Interfaces = std::vector<ConstRef<ConstPool::ClassInfo>>;

class Class {
public:
    constexpr static std::uint16_t MIN_CLASS_FILE_FORMAT_MAJOR_VERSION = 55;

    [[nodiscard]] static nonstd::expected<Class, std::variant<ParsingError, ConstPool::Error>> load(const std::filesystem::path& path);

    [[nodiscard]] nonstd::expected<std::string_view, ConstPool::Error> getClassName() const;
    [[nodiscard]] nonstd::expected<std::string_view, ConstPool::Error> getParentClassName() const;

    ver16 version;
    ConstPool constPool;
    AccessFlags accessFlags;
    ConstRef<ConstPool::ClassInfo> thisClass;
    ConstRef<ConstPool::ClassInfo> superClass;
    Interfaces interfaces;
    Fields fields;
    Methods methods;
    Attributes attributes;

protected:

    [[nodiscard]] nonstd::expected<void, ParsingError> readVersion(std::istream& in);
    [[nodiscard]] nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> readInterfaces(std::istream& in);
    [[nodiscard]] nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> readFields(std::istream& in);
    [[nodiscard]] nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> readMethods(std::istream& in);
    [[nodiscard]] nonstd::expected<Attributes, std::variant<ParsingError, ConstPool::Error>> readAttributes(std::istream& in) const;
};

}

#endif//MINI_JVM_TYPES_CLASS_HPP
