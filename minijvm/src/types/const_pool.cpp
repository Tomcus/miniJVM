#include "jvm/types/const_pool.hpp"
#include "jvm/utils.hpp"
#include "jvm/types/serialization.hpp"

#include <cstdint>
#include <fmt/core.h>
#include <istream>
#include <stdexcept>
#include <variant>
#include <typeinfo>

#include <fmtlog/fmtlog.h>

namespace jvm {

ConstPool ConstPool::load(std::istream& in) {
    ConstPool cp;
    cp.loadInstance(in);
    return cp;
}

template<>
std::string read(std::istream & in) {
    logd("Reading string");
    const auto stringSize = read<std::uint16_t>(in);
    std::string res{};
    res.resize(stringSize);
    if (!in.read(res.data(), stringSize).good()) {
        throw ConstPool::Error{fmt::format("Can't read string of length: {}. {}", stringSize, getIStreamErrorString(in))};
    }
    return res;
}

template<>
ConstPool::Tag read(std::istream & in) {
    logd("Reading ConstPool::Tag");
    return static_cast<ConstPool::Tag>(read<std::uint8_t>(in));
}

template<>
ConstPool::MethodRef read(std::istream & in) {
    logd("Reading ConstPool::MethodRef");
    return ConstPool::MethodRef{
        .classInfo = read<Index>(in),
        .nameAndType = read<Index>(in)
    };
}

template<>
ConstPool::FieldRef read(std::istream& in) {
    logd("Reading ConstPool::FieldRef");
    return ConstPool::FieldRef {
        .classInfo = read<Index>(in),
        .nameAndType = read<Index>(in)
    };
}

template<>
ConstPool::NameAndType read(std::istream & in) {
    logd("Reading ConstPool::NameAndType");
    return ConstPool::NameAndType {
        .name = read<Index>(in),
        .type = read<Index>(in)
    };
}

template<>
ConstPool::ClassInfo read(std::istream & in) {
    logd("Reading ConstPool::ClassInfo");
    return ConstPool::ClassInfo {
        .name = read<Index>(in)
    };
}

template<>
ConstPool::StringRef read(std::istream & in) {
    logd("Reading ConstPool::StringRef");
    return ConstPool::StringRef {
        .string = read<Index>(in)
    };
}

template<>
ConstPool::InvokeDynamic read(std::istream & in) {
    logd("Reading ConstPool::InvokeDynamic");
    return ConstPool::InvokeDynamic {
        .boostrapMethod = read<Index>(in),
        .nameAndType = read<Index>(in)
    };
}

template<>
ConstPool::MethodHandle::Type read(std::istream& in) {
    logd("Reading ConstPool::MethodHandle::Type");
    constexpr static std::uint8_t minValue = 1;
    constexpr static std::uint8_t maxValue = 9;
    const auto val = read<std::uint8_t>(in);
    check<ConstPool::Error>(val >= minValue && val <= maxValue, "MethodHandle kind should be value between 1 and 9");
    return static_cast<ConstPool::MethodHandle::Type>(val);
}

template<>
ConstPool::MethodHandle read(std::istream & in) {
    logd("Reading ConstPool::MethodHandle");
    return ConstPool::MethodHandle {
        .kind = read<ConstPool::MethodHandle::Type>(in),
        .referenceIndex = read<Index>(in)
    };
}

size_t ConstPool::size() const {
    return dataSize;
}

const ConstPool::Value& ConstPool::operator[](const std::size_t index) const {
    validateIndex(index);
    return data[index - 1];
}

void ConstPool::validateIndex(const std::size_t index) const {
    if (index == 0) {
        throw std::out_of_range("Const pool is indexed from 1. 0 shouldn't be accessed.");
    }
    if (index > data.size()) {
        throw std::out_of_range(fmt::format("Const pool index [{}] out of range (max is {})", index, data.size()));
    }
}

void ConstPool::loadInstance(std::istream& in) {
    const std::size_t constPoolMaxIndex = read<std::uint16_t>(in);
    check<Error>(constPoolMaxIndex != 0, "Const poll max index should be bigger then 0");
    dataSize = constPoolMaxIndex - 1;
    if (dataSize == 0) {
        fmt::print("Empty constpool");
        return;
    }

    data.reserve(dataSize);
    for (std::size_t i = 0; i < dataSize; ++i) {
        auto typeTag = read<ConstPool::Tag>(in);
        switch(typeTag) {
            case ConstPool::Tag::MethodRef:
                data.emplace_back(read<ConstPool::MethodRef>(in));
            break;
            case ConstPool::Tag::FieldRef:
                data.emplace_back(read<ConstPool::FieldRef>(in));
            break;
            case ConstPool::Tag::StringRef:
                data.emplace_back(read<ConstPool::StringRef>(in));
            break;
            case ConstPool::Tag::Class:
                data.emplace_back(read<ConstPool::ClassInfo>(in));
            break;
            case ConstPool::Tag::NameAndType:
                data.emplace_back(read<ConstPool::NameAndType>(in));
            break;
            case ConstPool::Tag::String:
                data.emplace_back(read<std::string>(in));
            break;
            case ConstPool::Tag::InvokeDynamic:
                data.emplace_back(read<ConstPool::InvokeDynamic>(in));
            break;
            case ConstPool::Tag::MethodHandle:
                data.emplace_back(read<ConstPool::MethodHandle>(in));
            break;
            default:
                throw Error{fmt::format("Unhandled tag: {:#02x}", static_cast<std::uint8_t>(typeTag))};
        }
    }
}

}
