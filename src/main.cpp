#include "types/class.hpp"
#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);
    auto classOrError = (jvm::Class::load("Add.class"));
    if (classOrError.isError()) {
        auto& er = classOrError.getError();
        spdlog::error("{}\n", er.getErrorMessage());
        return er.getErrorNumber();
    }
    return 0;
}