#include "types.hpp"
#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);
    try {
        const auto classFile = jvm::Class::load("Add.class");
    } catch (std::exception& e) {
        spdlog::error(e.what());
        return 1;
    }
    return 0;
}