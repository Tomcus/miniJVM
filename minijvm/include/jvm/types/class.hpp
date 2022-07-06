#pragma once

#include "jvm/types/const_pool.hpp"
#include "jvm/types/class/attribute.hpp"
#include "jvm/types/class/field.hpp"
#include "jvm/types/class/method.hpp"
#include "jvm/types/basic.hpp"
#include "jvm/types/frame.hpp"
#include "jvm/utils/ref.hpp"

#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <filesystem>
#include <optional>

namespace jvm {

struct ver16 {
    std::uint16_t major;
    std::uint16_t minor;
};

using Interfaces = std::vector<ConstRef<ConstPool::ClassInfo>>;

class ClassBase {
public:
    virtual ~ClassBase() { }
    virtual std::optional<Frame> resolveMethod(const std::string_view) const = 0;
    virtual std::optional<VariableType> resolveField(const std::string_view) const = 0;
};

class Class: public ClassBase {
public:
    constexpr static std::uint16_t MIN_CLASS_FILE_FORMAT_MAJOR_VERSION = 55;

    static Class load(const std::filesystem::path& path);

    std::string_view getClassName() const;
    std::string_view getParentClassName() const;

    std::optional<Frame> resolveMethod(const std::string_view) const final;
    std::optional<VariableType> resolveField(const std::string_view) const final;

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

    void readVersion(std::istream& in);
    void readInterfaces(std::istream& in);
    void readFields(std::istream& in);
    void readMethods(std::istream& in);
    Attributes readAttributes(std::istream& in) const;
};

class ReadingError: public std::runtime_error {
public:
    ReadingError(const std::string& message): runtime_error(message) { }
};

class ClassError: public std::runtime_error {
public:
    ClassError(const std::string& message): runtime_error(message) { }
};

}
