#include "loader.hpp"
#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);
    auto classOrError = (jvm::Loader::loadClassFile("Add.class"));
    if (classOrError.isError()) {
        auto& er = classOrError.getError();
        fmt::print("{}\n", er.getErrorMessage());
        return er.getErrorNumber();
    }
    auto& c = classOrError.getValue();
    return 0;
}