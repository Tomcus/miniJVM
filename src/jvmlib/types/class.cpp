#include "class.hpp"
#include "const_pool.hpp"
#include "const_pool_validation.hpp"
#include "const_pool_formating.hpp"
#include "serialization.hpp"

#include <cstdint>
#include <fstream>

namespace jvm {

template<>
ErrorOr<ConstPool::Tag> read(std::istream & in) noexcept {
    auto tagNum = TRY(read<std::uint8_t>(in));
    return static_cast<ConstPool::Tag>(tagNum);
}

template<>
ErrorOr<ConstPool::MethodRef> read(std::istream & in) noexcept {
    ConstPool::MethodRef ref{};
    ref.classInfo = TRY(read<ConstPool::Index>(in));
    ref.nameAndType = TRY(read<ConstPool::Index>(in));
    return ref;
}

template<>
ErrorOr<ConstPool::NameAndType> read(std::istream & in) noexcept {
    ConstPool::NameAndType nat{};
    nat.name = TRY(read<ConstPool::Index>(in));
    nat.type = TRY(read<ConstPool::Index>(in));
    return nat;
}

template<>
ErrorOr<ConstPool::ClassInfo> read(std::istream & in) noexcept {
    ConstPool::ClassInfo info{};
    info.name = TRY(read<ConstPool::Index>(in));
    return info;
}

template<>
ErrorOr<std::string> read(std::istream & in) noexcept {
    const auto stringSize = TRY(read<std::uint16_t>(in));
    std::string res{};
    res.resize(stringSize);
    if (!in.read(res.data(), stringSize).good()) {
        return Error{getIStreamErrorNumber(in), fmt::format("Can't read string of lenght: {}. {}", stringSize, getIStreamErrorString(in))};
    }
    return res;
}

ErrorOr<Class> Class::load(const std::filesystem::path& path) noexcept {
    CHECK(std::filesystem::exists(path));
    std::ifstream file(path);
    CHECK(file.good());

    const auto magic = TRY(read<std::uint32_t>(file));

    CHECK(magic == 0xcafebabe);

    Class res{};
    
    TRY(res.readVersion(file));
    TRY(res.readConstPool(file));


    return res;
}

Error Class::readVersion(std::istream& in) noexcept {
    version.minor = TRY(read<std::uint16_t>(in));
    version.major = TRY(read<std::uint16_t>(in));

    CHECK(version.minor == 0);
    CHECK(version.major >= 55);
    return {};
}

Error Class::readConstPool(std::istream& in) noexcept {
    const std::size_t constPoolMaxIndex = TRY(read<std::uint16_t>(in));
    CHECK(constPoolMaxIndex != 0);
    const std::size_t constPoolSize = constPoolMaxIndex - 1;
    if (constPoolSize == 0) {
        spdlog::warn("Empty constpool");
        return {};
    }

    constPool.resize(constPoolSize);
    for (std::size_t i = 0; i < constPoolSize; ++i) {
        auto typeTag = TRY(read<ConstPool::Tag>(in));
        switch(typeTag) {
            case ConstPool::Tag::MethodRef: {
                auto mr = TRY(read<ConstPool::MethodRef>(in));
                constPool.push_back(mr);
            }
            break;
            case ConstPool::Tag::Class:
                constPool.push_back(TRY(read<ConstPool::ClassInfo>(in)));
            break;
            case ConstPool::Tag::NameAndType:
                constPool.push_back(TRY(read<ConstPool::NameAndType>(in)));
            break;
            case ConstPool::Tag::String:
                constPool.push_back(TRY(read<std::string>(in)));
            break;
            default:
                return Error{fmt::format("Unhandled tag: {:#04x}", typeTag)};
        }
    }

    for (std::size_t i = 0; i < constPoolSize; ++i) {
        const auto& constPoolValue = constPool[i];
        spdlog::debug("Validating info at index {:#04x} with value {}", i + 1, format(constPoolValue));
        TRY(std::visit([&](const auto& val){
            return validate(val, constPool);
        }, constPoolValue));
    }

    return {};
}

}