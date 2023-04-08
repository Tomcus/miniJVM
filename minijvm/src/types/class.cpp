#include "jvm/types/class.hpp"
#include "jvm/types/basic.hpp"
#include "jvm/types/class/base.hpp"
#include "jvm/types/const_pool.hpp"
#include "jvm/types/serialization.hpp"
#include "jvm/utils.hpp"

#include <cstdint>
#include <fstream>
#include <istream>
#include <string>
#include <variant>

#include <fmtlog/fmtlog.h>

namespace jvm {

template<>
nonstd::expected<AccessFlags, ParsingError> read(std::istream& in) {
    auto raw = read<std::uint16_t>(in);
    PROPAGATE_ERROR(raw);
    return static_cast<AccessFlags>(*raw);
}

nonstd::expected<Class, std::variant<ParsingError, ConstPool::Error>> Class::load(const std::filesystem::path& path) {
    auto checkRes = check<ParsingError>(std::filesystem::exists(path), fmt::format("Class file {} doesn't exists.", path.c_str()));
    PROPAGATE_ERROR(checkRes);
    std::ifstream file(path);
    checkRes = check<ParsingError>(file.good(), "Class file couldn't be opened.");
    PROPAGATE_ERROR(checkRes);

    const auto magic = read<std::uint32_t>(file);

    constexpr static std::uint32_t cafeBabe = 0xcafebabe;
    checkRes = check<ParsingError>(magic == cafeBabe, "Class file magic bytes are not 'cafebabe'");
    PROPAGATE_ERROR(checkRes);

    Class res{};
    
    auto verRes = res.readVersion(file);
    PROPAGATE_ERROR(verRes);
    auto cp = ConstPool::load(file);
    PROPAGATE_ERROR(cp);
    res.constPool = *cp;
    auto af = read<AccessFlags>(file);
    PROPAGATE_ERROR(af);
    res.accessFlags = *af;
    auto thisClassIndex = read<Index>(file);
    PROPAGATE_ERROR(thisClassIndex);
    auto thisClassRef = res.constPool.getRef<ConstPool::ClassInfo>(*thisClassIndex);
    PROPAGATE_ERROR(thisClassRef);
    res.thisClass = *thisClassRef;
    auto superClassIndex = read<Index>(file);
    PROPAGATE_ERROR(superClassIndex);
    auto superClassRef = res.constPool.getRef<ConstPool::ClassInfo>(*superClassIndex);
    PROPAGATE_ERROR(superClassRef);
    res.superClass = *superClassRef;
    auto cpRes = res.readInterfaces(file);
    PROPAGATE_ERROR(checkRes);
    cpRes = res.readFields(file);
    PROPAGATE_ERROR(checkRes);
    cpRes = res.readMethods(file);
    PROPAGATE_ERROR(checkRes);
    auto attrs = res.readAttributes(file);
    PROPAGATE_ERROR(attrs);
    res.attributes = *attrs;

    int eofCheck;
    file >> eofCheck;
    checkRes = check<ParsingError>(file.eof(), "Class file doesn't reach eof after it was parsed.");
    PROPAGATE_ERROR(checkRes);

    return res;
}

nonstd::expected<void, ParsingError> Class::readVersion(std::istream& in) {
    logd("Reading version");
    auto minor = read<std::uint16_t>(in);
    PROPAGATE_ERROR(minor);
    version.minor = *minor;
    auto major = read<std::uint16_t>(in);
    PROPAGATE_ERROR(major);
    version.major = *major;

    auto res = check<ParsingError>(version.minor == 0, "Wrong minimal version of class file format.");
    PROPAGATE_ERROR(res);
    res = check<ParsingError>(version.major >= MIN_CLASS_FILE_FORMAT_MAJOR_VERSION, fmt::format("Minimal major version of class file format ({}) not satisfied", MIN_CLASS_FILE_FORMAT_MAJOR_VERSION));
    PROPAGATE_ERROR(res);
    return {};
}

nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> Class::readInterfaces(std::istream& in) {
    logd("Reading Interfaces");
    const auto interfaceCount = read<std::uint16_t>(in);
    PROPAGATE_ERROR(interfaceCount);
    interfaces.reserve(*interfaceCount);
    for (std::size_t i = 0; i < *interfaceCount; ++i) {
        auto classInfoIndex = read<Index>(in);
        PROPAGATE_ERROR(classInfoIndex);
        auto data = constPool.getRef<ConstPool::ClassInfo>(*classInfoIndex);
        PROPAGATE_ERROR(data);
        interfaces.emplace_back(*data);
    }
    return {};
}

nonstd::expected<std::string_view, ConstPool::Error> Class::getClassName() const {
    auto stringClassName = constPool.getRef<std::string>(thisClass->name);
    PROPAGATE_ERROR(stringClassName);
    return ***stringClassName;
}

nonstd::expected<std::string_view, ConstPool::Error> Class::getParentClassName() const {
    auto stringClassName = constPool.getRef<std::string>(superClass->name);
    PROPAGATE_ERROR(stringClassName);
    return ***stringClassName;
}

nonstd::expected<Attributes, std::variant<ParsingError, ConstPool::Error>> Class::readAttributes(std::istream& in) const {
    logd("Reading attributes");
    Attributes attrs;
    const auto attrsCount = read<std::uint16_t>(in);
    PROPAGATE_ERROR(attrsCount);
    attrs.reserve(*attrsCount);
    for (std::size_t i = 0; i < *attrsCount; ++i) {
        Attribute attr;
        auto nameIndex = read<Index>(in);
        PROPAGATE_ERROR(nameIndex);
        auto nameRef = constPool.getRef<std::string>(*nameIndex);
        PROPAGATE_ERROR(nameRef);
        attr.name = *nameRef;
        const auto attrDataLength = read<std::uint32_t>(in);
        PROPAGATE_ERROR(attrDataLength);
        attr.data.resize(*attrDataLength);
        if (*attrDataLength > 0) {
            in.read(reinterpret_cast<char *>(attr.data.data()), *attrDataLength);
        }
        attrs.emplace_back(attr);
    }
    return attrs;
}

nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> Class::readFields(std::istream& in) {
    logd("Reading Fields");
    const auto fieldsCount = read<std::uint16_t>(in);
    PROPAGATE_ERROR(fieldsCount);
    fields.reserve(*fieldsCount);
    for (std::size_t i = 0; i < *fieldsCount; ++i) {
        auto inputAccessFlags = read<jvm::AccessFlags>(in);
        PROPAGATE_ERROR(inputAccessFlags);
        auto nameIndex = read<Index>(in);
        PROPAGATE_ERROR(nameIndex);
        auto nameRef = constPool.getRef<std::string>(*nameIndex);
        PROPAGATE_ERROR(nameRef);
        auto dataIndex = read<Index>(in);
        PROPAGATE_ERROR(dataIndex);
        auto attrs = readAttributes(in);
        PROPAGATE_ERROR(attrs);
        fields.emplace_back(Field{
            *inputAccessFlags,
            *nameRef,
            *dataIndex,
            *attrs
        });
    }
    return {};
}

nonstd::expected<void, std::variant<ParsingError, ConstPool::Error>> Class::readMethods(std::istream& in) {
    logd("Reading Methods");
    const auto methodsCount = read<std::uint16_t>(in);
    PROPAGATE_ERROR(methodsCount);
    methods.reserve(*methodsCount);
    for (std::size_t i = 0; i < *methodsCount; ++i) {
        auto inputAccessFlags = read<jvm::AccessFlags>(in);
        PROPAGATE_ERROR(inputAccessFlags);
        auto nameIndex = read<Index>(in);
        PROPAGATE_ERROR(nameIndex);
        auto nameRef = constPool.getRef<std::string>(*nameIndex);
        PROPAGATE_ERROR(nameRef);
        auto dataIndex = read<Index>(in);
        PROPAGATE_ERROR(dataIndex);
        auto attrs = readAttributes(in);
        PROPAGATE_ERROR(attrs);
        methods.emplace_back(Method{
            *inputAccessFlags,
            *nameRef,
            *dataIndex,
            *attrs
        });
    }
    return {};
}

}
