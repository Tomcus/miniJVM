#ifndef MINI_JVM_LOADER_HPP
#define MINI_JVM_LOADER_HPP

#include <filesystem>

#include "types.hpp"
#include "error/error.hpp"

namespace jvm {

class Loader {
public:
static ErrorOr<Class> loadClassFile(const std::filesystem::path&) noexcept;

};

}

#endif//MINI_JVM_LOADER_HPP