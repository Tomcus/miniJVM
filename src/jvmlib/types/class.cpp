#include "class.hpp"
#include "basic.hpp"
#include "const_pool.hpp"
#include "serialization.hpp"

#include <cstdint>
#include <fstream>
#include <istream>
#include <string>
#include <variant>

namespace jvm {

template<>
ConstPool::Tag read(std::istream & in) {
    auto tagNum = read<std::uint8_t>(in);
    return static_cast<ConstPool::Tag>(tagNum);
}

template<>
ConstPool::MethodRef read(std::istream & in) {
    ConstPool::MethodRef ref{};
    ref.classInfo = read<ConstPool::Index>(in) - 1;
    ref.nameAndType = read<ConstPool::Index>(in) - 1;
    return ref;
}

template<>
ConstPool::NameAndType read(std::istream & in) {
    ConstPool::NameAndType nat{};
    nat.name = read<ConstPool::Index>(in) - 1;
    nat.type = read<ConstPool::Index>(in) - 1;
    return nat;
}

template<>
ConstPool::ClassInfo read(std::istream & in) {
    ConstPool::ClassInfo info{};
    info.name = read<ConstPool::Index>(in) - 1;
    return info;
}

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
        attr.attrNameIndex = read<ConstPool::Index>(in) - 1;
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
            .nameIndex = static_cast<ConstPool::Index>(read<ConstPool::Index>(in) - 1),
            .descriptorIndex = static_cast<ConstPool::Index>(read<ConstPool::Index>(in) - 1),
            .attributes = read<Attributes>(in)
        });
    }
    return fields;
}

template<>
std::string read(std::istream & in) {
    const auto stringSize = read<std::uint16_t>(in);
    std::string res{};
    res.resize(stringSize);
    if (!in.read(res.data(), stringSize).good()) {
        throw jvm::ReadingError{fmt::format("Can't read string of length: {}. {}", stringSize, getIStreamErrorString(in))};
    }
    return res;
}

Class Class::load(const std::filesystem::path& path) {
    ConstPool::check<ClassError>(std::filesystem::exists(path), fmt::format("Class file {} doesn't exists.", path.c_str()));
    std::ifstream file(path);
    ConstPool::check<ClassError>(file.good(), "Class file couldn't be opened.");

    const auto magic = read<std::uint32_t>(file);

    constexpr static std::uint32_t cafeBabe = 0xcafebabe;
    ConstPool::check<ClassError>(magic == cafeBabe, "Class file magic bytes are not 'cafebabe'");

    Class res{};
    
    res.readVersion(file);
    res.readConstPool(file);
    res.accessFlags = read<Class::AccessFlags>(file);
    res.thisClass = read<ConstPool::Index>(file) - 1;
    res.superClass = read<ConstPool::Index>(file) - 1;
    res.readInterfaces(file);
    res.fields = read<Fields>(file);
    res.methods = read<Fields>(file);
    res.attributes = read<Attributes>(file);

    int check;
    file >> check;
    ConstPool::check<ClassError>(file.eof(), "Class file doesn't reach eof after it was parsed.");

    return res;
}

void Class::readVersion(std::istream& in) {
    version.minor = read<std::uint16_t>(in);
    version.major = read<std::uint16_t>(in);

    ConstPool::check<ReadingError>(version.minor == 0, "Wrong minimal version of class file format.");
    ConstPool::check<ReadingError>(version.major >= MIN_CLASS_FILE_FORMAT_MAJOR_VERSION, fmt::format("Minimal major version of class file format ({}) not satisfied", MIN_CLASS_FILE_FORMAT_MAJOR_VERSION));
}

void Class::readConstPool(std::istream& in) {
    const std::size_t constPoolMaxIndex = read<std::uint16_t>(in);
    ConstPool::check<ClassError>(constPoolMaxIndex != 0, "Const poll max index should be bigger then 0");
    const std::size_t constPoolSize = constPoolMaxIndex - 1;
    if (constPoolSize == 0) {
        spdlog::warn("Empty constpool");
        return;
    }

    constPool.reserve(constPoolSize);
    for (std::size_t i = 0; i < constPoolSize; ++i) {
        auto typeTag = read<ConstPool::Tag>(in);
        switch(typeTag) {
            case ConstPool::Tag::MethodRef: {
                constPool.emplace_back(read<ConstPool::MethodRef>(in));
            }
            break;
            case ConstPool::Tag::Class:
                constPool.emplace_back(read<ConstPool::ClassInfo>(in));
            break;
            case ConstPool::Tag::NameAndType:
                constPool.emplace_back(read<ConstPool::NameAndType>(in));
            break;
            case ConstPool::Tag::String:
                constPool.emplace_back(read<std::string>(in));
            break;
            default:
                throw ConstPool::Error{fmt::format("Unhandled tag: {:#04x}", static_cast<std::uint8_t>(typeTag))};
        }
    }
}

void Class::readInterfaces(std::istream& in) {
    const auto interfaceCount = read<std::uint16_t>(in);
    interfaces.reserve(interfaceCount);
    for (std::size_t i = 0; i < interfaceCount; ++i) {
        interfaces.push_back(read<ConstPool::Index>(in) - 1);
    }
}

std::string_view Class::getClassName() const {
    auto& cpReference = constPool[thisClass];
    if (std::holds_alternative<ConstPool::ClassInfo>(cpReference)) {
        auto& classInfo = std::get<ConstPool::ClassInfo>(cpReference);
        auto& cpReferenceToString = constPool[classInfo.name];
        if (std::holds_alternative<BasicType>(cpReferenceToString) &&
            std::holds_alternative<std::string>(std::get<BasicType>(cpReferenceToString))) {
            return std::get<std::string>(std::get<BasicType>(cpReferenceToString));
        }
    }
    throw ClassError{"Can't get the class name."};
}

std::string_view Class::getParentClassName() const {
    auto& cpReference = constPool[superClass];
    if (std::holds_alternative<ConstPool::ClassInfo>(cpReference)) {
        auto& classInfo = std::get<ConstPool::ClassInfo>(cpReference);
        auto& cpReferenceToString = constPool[classInfo.name];
        if (std::holds_alternative<BasicType>(cpReferenceToString) &&
            std::holds_alternative<std::string>(std::get<BasicType>(cpReferenceToString))) {
            return std::get<std::string>(std::get<BasicType>(cpReferenceToString));
        }
    }
    throw ClassError{"Can't get the class's super class name."};
}

}