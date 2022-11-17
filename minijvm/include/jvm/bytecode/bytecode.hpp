#pragma once

#include <jvm/bytecode/simple/forward.hpp>

#include <iostream>

namespace jvm::bytecode {
    Instructions parse(std::istream& input);
}
