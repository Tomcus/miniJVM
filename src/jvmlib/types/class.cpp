#include "class.hpp"
#include "const_pool.hpp"
#include "serialization.hpp"
#include "utils.hpp"

#include <cstdint>
#include <fstream>
#include <istream>
#include <string>
#include <variant>

namespace jvm {

template<>
Class::AccessFlags read(std::istream& in) {
    return static_cast<Class::AccessFlags>(read<std::uint16_t>(in));
}

template<>
Attributes read(std::istream& in) {
    Attributes attrs;
    const auto attrsCount = read<std::uint16_t>(in);
    attrs.reserve(attrsCount);
    for (std::size_t i = 0; i < attrsCount; ++i) {
        Attribute attr;
        attr.attrNameIndex = read<Index>(in);
        const auto attrDataLength = read<std::uint32_t>(in);
        attr.data.resize(attrDataLength);
        if (attrDataLength > 0) {
            in.read(reinterpret_cast<char *>(attr.data.data()), attrDataLength);
        }
        attrs.emplace_back(std::move(attr));
    }
    return attrs;
}

template<>
Fields read(std::istream& in) {
    std::vector<Field> fields;
    const auto fieldsCount = read<std::uint16_t>(in);
    fields.reserve(fieldsCount);
    for (std::size_t i = 0; i < fieldsCount; ++i) {
        fields.emplace_back(Field{
            .flags = read<std::uint16_t>(in),
            .nameIndex = read<Index>(in),
            .descriptorIndex = read<Index>(in),
            .attributes = read<Attributes>(in)
        });
    }
    return fields;
}

template<>
Methods read(std::istream& in) {
    std::vector<Method> methods;
    const auto methodsCount = read<std::uint16_t>(in);
    methods.reserve(methodsCount);
    for (std::size_t i = 0; i < methodsCount; ++i) {
        methods.emplace_back(Method{
            .flags = read<std::uint16_t>(in),
            .nameIndex = read<Index>(in),
            .descriptorIndex = read<Index>(in),
            .attributes = read<Attributes>(in)
        });
    }
    return methods;
}

Class Class::load(const std::filesystem::path& path) {
    check<ClassError>(std::filesystem::exists(path), fmt::format("Class file {} doesn't exists.", path.c_str()));
    std::ifstream file(path);
    check<ClassError>(file.good(), "Class file couldn't be opened.");

    const auto magic = read<std::uint32_t>(file);

    constexpr static std::uint32_t cafeBabe = 0xcafebabe;
    check<ClassError>(magic == cafeBabe, "Class file magic bytes are not 'cafebabe'");

    Class res{};
    
    res.readVersion(file);
    res.constPool = ConstPool::load(file);
    res.accessFlags = read<Class::AccessFlags>(file);
    res.thisClass = read<Index>(file);
    res.superClass = read<Index>(file);
    res.readInterfaces(file);
    res.fields = read<Fields>(file);
    res.methods = read<Methods>(file);
    res.attributes = read<Attributes>(file);

    int eofCheck;
    file >> eofCheck;
    check<ClassError>(file.eof(), "Class file doesn't reach eof after it was parsed.");

    return res;
}

void Class::readVersion(std::istream& in) {
    version.minor = read<std::uint16_t>(in);
    version.major = read<std::uint16_t>(in);

    check<ReadingError>(version.minor == 0, "Wrong minimal version of class file format.");
    check<ReadingError>(version.major >= MIN_CLASS_FILE_FORMAT_MAJOR_VERSION, fmt::format("Minimal major version of class file format ({}) not satisfied", MIN_CLASS_FILE_FORMAT_MAJOR_VERSION));
}

void Class::readInterfaces(std::istream& in) {
    const auto interfaceCount = read<std::uint16_t>(in);
    interfaces.reserve(interfaceCount);
    for (std::size_t i = 0; i < interfaceCount; ++i) {
        interfaces.push_back(read<Index>(in));
    }
}

std::string_view Class::getClassName() const {
    const auto& thisClassInfo = constPool.get<ConstPool::ClassInfo>(thisClass);
    const auto& stringClassName = constPool.get<std::string>(thisClassInfo.name);
    return stringClassName;
}

std::string_view Class::getParentClassName() const {
    const auto& superClassInfo = constPool.get<ConstPool::ClassInfo>(superClass);
    const auto& stringClassName = constPool.get<std::string>(superClassInfo.name);
    return stringClassName;
}

}