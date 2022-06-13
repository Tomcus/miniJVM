#include <exception>
#include "jvm/types/class.hpp"

int main() {
    try {
        const auto classFile = jvm::Class::load("Add.class");
    } catch (std::exception& e) {
        fmt::print("{}", e.what());
        return 1;
    }
    return 0;
}