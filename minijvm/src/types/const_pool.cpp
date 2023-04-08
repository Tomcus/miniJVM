#include "jvm/types/const_pool.hpp"
#include "jvm/types/basic.hpp"
#include "jvm/utils.hpp"
#include "jvm/types/serialization.hpp"

#include <cstdint>
#include <fmt/core.h>
#include <istream>
#include <stdexcept>
#include <variant>
#include <typeinfo>

#include <fmtlog/fmtlog.h>
#include <nonstd/expected.hpp>

namespace jvm {

nonstd::expected<ConstPool, ParsingError> ConstPool::load(std::istream& in) {
    ConstPool cp;
    auto res = cp.loadInstance(in);
    PROPAGATE_ERROR(res);
    return cp;
}

template<>
nonstd::expected<std::string, ParsingError> read(std::istream & in) {
    logd("Reading string");
    const auto stringSize = read<std::uint16_t>(in);
    PROPAGATE_ERROR(stringSize);
    std::string res{};
    res.resize(*stringSize);
    if (!in.read(res.data(), *stringSize).good()) {
        return nonstd::make_unexpected(ParsingError::cant_read_from_istream(in, "string"));
    }
    return res;
}

template<>
nonstd::expected<ConstPool::Tag, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::Tag");
    auto res = read<std::uint8_t>(in);
    PROPAGATE_ERROR(res);
    return static_cast<ConstPool::Tag>(*res);
}

template<>
nonstd::expected<ConstPool::MethodRef, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::MethodRef");
    auto classInfoIndex = read<Index>(in);
    PROPAGATE_ERROR(classInfoIndex);
    auto nameAndTypeIndex = read<Index>(in);
    PROPAGATE_ERROR(nameAndTypeIndex);
    return ConstPool::MethodRef{
        .classInfo = *classInfoIndex,
        .nameAndType = *nameAndTypeIndex
    };
}

template<>
nonstd::expected<ConstPool::FieldRef, ParsingError> read(std::istream& in) {
    logd("Reading ConstPool::FieldRef");
    auto classInfoIndex = read<Index>(in);
    PROPAGATE_ERROR(classInfoIndex);
    auto nameAndTypeIndex = read<Index>(in);
    PROPAGATE_ERROR(nameAndTypeIndex);
    return ConstPool::FieldRef {
        .classInfo = *classInfoIndex,
        .nameAndType = *nameAndTypeIndex
    };
}

template<>
nonstd::expected<ConstPool::NameAndType, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::NameAndType");
    auto nameIndex = read<Index>(in);
    PROPAGATE_ERROR(nameIndex);
    auto typeIndex = read<Index>(in);
    PROPAGATE_ERROR(typeIndex);
    return ConstPool::NameAndType {
        .name = *nameIndex,
        .type = *typeIndex
    };
}

template<>
nonstd::expected<ConstPool::ClassInfo, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::ClassInfo");
    auto classInfoIndex = read<Index>(in);
    PROPAGATE_ERROR(classInfoIndex);
    return ConstPool::ClassInfo {
        .name = *classInfoIndex
    };
}

template<>
nonstd::expected<ConstPool::StringRef, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::StringRef");
    auto stringIndex = read<Index>(in);
    PROPAGATE_ERROR(stringIndex);
    return ConstPool::StringRef {
        .string = *stringIndex
    };
}

template<>
nonstd::expected<ConstPool::InvokeDynamic, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::InvokeDynamic");
    auto methodIndex = read<Index>(in);
    PROPAGATE_ERROR(methodIndex);
    auto nameAndTypeIndex = read<Index>(in);
    PROPAGATE_ERROR(nameAndTypeIndex);
    return ConstPool::InvokeDynamic {
        .boostrapMethod = *methodIndex,
        .nameAndType = *nameAndTypeIndex
    };
}

template<>
nonstd::expected<ConstPool::MethodHandle::Type, ParsingError> read(std::istream& in) {
    logd("Reading ConstPool::MethodHandle::Type");
    constexpr static std::uint8_t minValue = 1;
    constexpr static std::uint8_t maxValue = 9;
    const auto val = read<std::uint8_t>(in);
    PROPAGATE_ERROR(val);
    auto checkRes = check<ParsingError>(*val >= minValue && *val <= maxValue, "MethodHandle kind should be value between 1 and 9");
    PROPAGATE_ERROR(checkRes);
    return static_cast<ConstPool::MethodHandle::Type>(*val);
}

template<>
nonstd::expected<ConstPool::MethodHandle, ParsingError> read(std::istream & in) {
    logd("Reading ConstPool::MethodHandle");
    auto kind = read<ConstPool::MethodHandle::Type>(in);
    PROPAGATE_ERROR(kind);
    auto referenceIndex = read<Index>(in);
    PROPAGATE_ERROR(referenceIndex);
    return ConstPool::MethodHandle {
        .kind = *kind,
        .referenceIndex = *referenceIndex
    };
}

size_t ConstPool::size() const {
    return dataSize;
}

nonstd::expected<jvm::ConstRef<ConstPool::Value>, ConstPool::Error> ConstPool::operator[](const std::size_t index) const {
    auto res = validateIndex(index);
    PROPAGATE_ERROR(res);
    return data[index - 1];
}

nonstd::expected<void, ConstPool::Error> ConstPool::validateIndex(const std::size_t index) const {
    if (index == 0) {
        return nonstd::make_unexpected(
            Error{.message = "Const pool is indexed from 1. 0 shouldn't be accessed."}
        );
    }
    if (index > data.size()) {
        return nonstd::make_unexpected(
            Error{.message = fmt::format("Const pool index [{}] out of range (max is {})", index, data.size())}
        );
    }
    return {};
}

nonstd::expected<void, ParsingError> ConstPool::loadInstance(std::istream& in) {
    const auto constPoolMaxIndex = read<std::uint16_t>(in);
    PROPAGATE_ERROR(constPoolMaxIndex);
    check<Error>(*constPoolMaxIndex != 0, "Const poll max index should be bigger then 0");
    dataSize = *constPoolMaxIndex - 1;
    if (dataSize == 0) {
        fmt::print("Empty constpool");
        return {};
    }

    data.reserve(dataSize);
    for (std::size_t i = 0; i < dataSize; ++i) {
        auto typeTag = read<ConstPool::Tag>(in);
        PROPAGATE_ERROR(typeTag);
        switch(*typeTag) {
            case ConstPool::Tag::MethodRef: {
                auto methodRef = read<ConstPool::MethodRef>(in);
                PROPAGATE_ERROR(methodRef);
                data.emplace_back(*methodRef);
                break;
            }
            case ConstPool::Tag::FieldRef: {
                auto fieldRef = read<ConstPool::FieldRef>(in);
                PROPAGATE_ERROR(fieldRef);
                data.emplace_back(*fieldRef);
                break;
            }
            case ConstPool::Tag::StringRef: {
                auto stringRef = read<ConstPool::StringRef>(in);
                PROPAGATE_ERROR(stringRef);
                data.emplace_back(*stringRef);
                break;
            }
            case ConstPool::Tag::Class: {
                auto classInfo = read<ConstPool::ClassInfo>(in);
                PROPAGATE_ERROR(classInfo);
                data.emplace_back(*classInfo);
                break;
            }
            case ConstPool::Tag::NameAndType: {
                auto nameAndType = read<ConstPool::NameAndType>(in);
                PROPAGATE_ERROR(nameAndType);
                data.emplace_back(*nameAndType);
                break;
            }
            case ConstPool::Tag::String: {
                auto cpString = read<std::string>(in);
                PROPAGATE_ERROR(cpString);
                data.emplace_back(*cpString);
                break;
            }
            case ConstPool::Tag::InvokeDynamic: {
                auto invokeDynamic = read<ConstPool::InvokeDynamic>(in);
                PROPAGATE_ERROR(invokeDynamic);
                data.emplace_back(*invokeDynamic);
                break;
            }
            case ConstPool::Tag::MethodHandle: {
                auto methodHandle = read<ConstPool::MethodHandle>(in);
                PROPAGATE_ERROR(methodHandle);
                data.emplace_back(*methodHandle);
                break;
            }
            default:
                return nonstd::make_unexpected(ParsingError {
                    .message = fmt::format("Unhandled tag: {:#02x}", static_cast<std::uint8_t>(*typeTag))
                });
        }
    }
    return {};
}

}
