#include "loader.hpp"
#include "types/basic.hpp"
#include "types/class.hpp"
#include "types/const.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <istream>
#include <iterator>
#include <string_view>
#include <span>
#include <concepts>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

std::ifstream open(const std::filesystem::path& path) noexcept {
    return std::ifstream(path.c_str());
}

constexpr static int ISTREAM_EOF_ERROR = 100;
constexpr static int ISTREAM_FAIL_ERROR = 101;
constexpr static int ISTREAM_BAD_ERROR = 102;

template<typename ReturnType>
jvm::ErrorOr<ReturnType> read(std::istream&) noexcept {
    return jvm::Error{-1, "Unsupported type to read."};
}

inline int getIStreamErrorNumber(const std::istream& in) {
    if (in.eof()) {
        return ISTREAM_EOF_ERROR;
    }
    if (in.fail()) {
        return ISTREAM_FAIL_ERROR;
    }
    if (in.bad()) {
        return ISTREAM_BAD_ERROR;
    }
    return -1;
}

const std::string_view getIStreamErrorString(const std::istream& in) {
    if (in.eof()) {
        return "End of file reached.";
    }
    if (in.fail()) {
        return "Rocoverable error reached. Wrong data format read.";
    }
    if (in.bad()) {
        return "Unrecoverable error reached.";
    }
    return "No error";
}

template<std::unsigned_integral ReturnType> 
jvm::ErrorOr<ReturnType> read(std::istream& in) noexcept {
    static std::array<char, 64> readBuffer{};
    if (!in.read(readBuffer.data(), sizeof(ReturnType))) {
        return jvm::Error{getIStreamErrorNumber(in), fmt::format("Can't read the unsigned integer of size {} bytes. {}", sizeof(ReturnType), getIStreamErrorString(in))};
    }
    ReturnType res = 0;
    for (std::size_t i = 0; i < sizeof(ReturnType); ++i) {
        res = res << 8;
        res += static_cast<std::uint8_t>(readBuffer[i]);
    }
    spdlog::debug("Read {}-bytes unsigned integer with value: {:#04x}", sizeof(ReturnType), res);
    return res;
}

template<>
jvm::ErrorOr<jvm::tag> read(std::istream& in) noexcept {
    const auto res = TRY(read<std::uint8_t>(in));
    return static_cast<jvm::tag>(res);
}

template<>
jvm::ErrorOr<jvm::AccessFlags> read(std::istream& in) noexcept {
    const auto res = TRY(read<std::uint16_t>(in));
    return static_cast<jvm::AccessFlags>(res);
}

template<>
jvm::ErrorOr<jvm::ver16> read(std::istream& in) noexcept {
    jvm::ver16 res{};
    res.minor = TRY(read<std::uint16_t>(in));
    res.major = TRY(read<std::uint16_t>(in));
    return res;
}

template<>
jvm::ErrorOr<std::string> read(std::istream& in) noexcept {
    std::string res{};
    const auto len = TRY(read<std::uint16_t>(in));
    res.resize(len);
    if (len > 0 && !in.read(res.data(), len)) {
        return jvm::Error{-1, "Error while reading string from class file."};
    }
    spdlog::debug("Read string: {}", res);
    return res;
}

template<>
jvm::ErrorOr<jvm::Attribute> read(std::istream& in) noexcept {
    jvm::Attribute attr{};
    attr.attrNameIndex = TRY(read<std::uint16_t>(in));
    const auto dataLen = TRY(read<std::uint32_t>(in));
    attr.data.resize(dataLen);
    if (!in.read(reinterpret_cast<char *>(attr.data.data()), dataLen)) {
        return jvm::Error{-1, "Unable to read attribute data"};
    }
    spdlog::debug("Read attribute data: {}", fmt::join(attr.data, ", "));
    return attr;
}

template<>
jvm::ErrorOr<jvm::Attributes> read(std::istream& in) noexcept {
    const auto attributesSize = TRY(read<std::uint16_t>(in));
    spdlog::debug("About to read attributes data of size {}.", attributesSize);
    jvm::Attributes res{};
    res.reserve(attributesSize);
    for (std::size_t i = 0; i < attributesSize; ++i) {
        auto attr = TRY(read<jvm::Attribute>(in));
        res.push_back(attr);
    }
    return res;
}

template<>
jvm::ErrorOr<jvm::Field> read(std::istream& in) noexcept {
    jvm::Field field{};
    field.flags = TRY(read<std::uint16_t>(in));
    field.nameIndex = TRY(read<jvm::ConstPollIndex>(in));
    field.descriptorIndex = TRY(read<jvm::ConstPollIndex>(in));
    field.attributes = TRY(read<jvm::Attributes>(in));
    return field;
}

template<>
jvm::ErrorOr<jvm::Fields> read(std::istream& in) noexcept {
    const auto fieldSize = TRY(read<std::uint16_t>(in));
    spdlog::debug("About to read fields data of size {}.", fieldSize);
    jvm::Fields res{};
    res.reserve(fieldSize);
    for (std::size_t i = 0; i < fieldSize; ++i) {
        jvm::Field field = TRY(read<jvm::Field>(in));
        res.push_back(field);
    }
    return res;
}

template<>
jvm::ErrorOr<jvm::Interfaces> read(std::istream& in) noexcept {
    jvm::Interfaces res{};
    const std::size_t interfacesLen = TRY(read<std::uint16_t>(in));
    spdlog::debug("About to read interfaces of size {}.", interfacesLen);
    res.reserve(interfacesLen);
    for (std::size_t i = 0; i < interfacesLen; ++i) {
        const auto reference = TRY(read<std::uint16_t>(in));
        res.push_back(reference);
    }
    return res;
}

template<>
jvm::ErrorOr<jvm::ConstPool> read(std::istream& in) noexcept {
    const auto readPoolSize = TRY(read<std::uint16_t>(in)) - 1;
    const auto constPoolSize = static_cast<std::size_t>(readPoolSize);
    jvm::ConstPool res{};
    res.reserve(constPoolSize);
    for (std::size_t i = 0; i < constPoolSize; ++i) {
        const auto tag = TRY(read<jvm::tag>(in));
        jvm::ConstPoolValue cpv{};
        cpv.valTag = tag;
        switch (tag) {
            case jvm::tag::String:
                cpv.string = TRY(read<std::string>(in));
            break;
            case jvm::tag::Name:
                cpv.nameIndex = TRY(read<jvm::ConstPollIndex>(in));
            break;
            case jvm::tag::StringRef:
                cpv.stringIndex = TRY(read<jvm::ConstPollIndex>(in));
            break;
            case jvm::tag::Class:
            case jvm::tag::NameAndType:
                cpv.classIndex = TRY(read<jvm::ConstPollIndex>(in));
                cpv.nameAndTypeIndex = TRY(read<jvm::ConstPollIndex>(in));
            break;
            case jvm::tag::NameAndDesc:
                cpv.nameIndex = TRY(read<jvm::ConstPollIndex>(in));
                cpv.descIndex = TRY(read<jvm::ConstPollIndex>(in));
            break;
            default:
                return jvm::Error{-1, fmt::format("loadClassFileConstPool: Unknown tag ({:#04x})", tag)};
        }
        res.emplace_back(std::move(cpv));
    }
    return res;
}

template<typename ReturnType>
jvm::ErrorOr<ReturnType> readIgnoreEof(std::istream& in) noexcept {
    auto res = read<ReturnType>(in);
    if (res.isError()) {
        if (res.getErrorNumber() != ISTREAM_EOF_ERROR) {
            return res;
        }
        return ReturnType{};
    }
    return res;
}

jvm::Error loadClassFileHeader(std::istream& in) noexcept {
    // Read and check "cafe babe" prefix
    std::array<char, 4> readBuffer;
    if (!in.read(readBuffer.data(), 4)) {
        return jvm::Error{2, "Couldn't read \"cafe babe\" bytes."};
    }
    static constexpr jvm::byte cafeBabeRef[] = {0xca, 0xfe, 0xba, 0xbe};
    for (std::size_t i = 0; i < 4; ++i) {
        if (static_cast<jvm::byte>(readBuffer[i]) != cafeBabeRef[i]) {
            return jvm::Error{3, "Wrong class file name format. No cafe babe prefix."};
        }
    }
    return {};
}

namespace jvm {

ErrorOr<Class> Loader::loadClassFile(const std::filesystem::path& path) noexcept {
    auto file = open(path);
    if (!file.good()) {
        return Error{1, fmt::format("Unable to open file: {}.", path.c_str())};
    }

    Class c;

    TRY(loadClassFileHeader(file));
    c.version = TRY(read<ver16>(file));
    c.constpool = TRY(read<ConstPool>(file));
    c.thisClass = TRY(read<ConstPollIndex>(file));
    c.superClass = TRY(read<ConstPollIndex>(file));
    c.interfaces = TRY(read<Interfaces>(file));
    c.fields = TRY(read<Fields>(file));
    c.methods = TRY(read<Fields>(file));
    c.attributes = TRY(read<Attributes>(file));

    return c;
}

}