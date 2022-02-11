#include "class.hpp"
#include "const_pool.hpp"
#include "const_pool_validation.hpp"
#include "serialization.hpp"

#include <cstdint>
#include <fstream>

namespace jvm {

template<>
ConstPool::Tag read(std::istream & in) {
    auto tagNum = read<std::uint8_t>(in);
    return static_cast<ConstPool::Tag>(tagNum);
}

template<>
ConstPool::MethodRef read(std::istream & in) {
    ConstPool::MethodRef ref{};
    ref.classInfo = read<ConstPool::Index>(in);
    ref.nameAndType = read<ConstPool::Index>(in);
    return ref;
}

template<>
ConstPool::NameAndType read(std::istream & in) {
    ConstPool::NameAndType nat{};
    nat.name = read<ConstPool::Index>(in);
    nat.type = read<ConstPool::Index>(in);
    return nat;
}

template<>
ConstPool::ClassInfo read(std::istream & in) {
    ConstPool::ClassInfo info{};
    info.name = read<ConstPool::Index>(in);
    return info;
}

template<>
std::string read(std::istream & in) {
    const auto stringSize = read<std::uint16_t>(in);
    std::string res{};
    res.resize(stringSize);
    if (!in.read(res.data(), stringSize).good()) {
        throw jvm::ReadingError{fmt::format("Can't read string of lenght: {}. {}", stringSize, getIStreamErrorString(in))};
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

    constPool.resize(constPoolSize);
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

}