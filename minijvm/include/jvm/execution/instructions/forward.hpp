#pragma once

#include "jvm/execution/execution.hpp"

#define INSTRUCTION_INVOKE_FUNCTION [[maybe_unused]] Stack& stack

namespace jvm::op {

struct iconst_m1;
struct iconst_0;
struct iconst_1;
struct iconst_2;
struct iconst_3;
struct iconst_4;
struct iconst_5;

}
