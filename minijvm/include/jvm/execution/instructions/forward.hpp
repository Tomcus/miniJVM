#pragma once

#include <cstdint>
#include <span>

#include "jvm/execution/execution.hpp"
#include "jvm/execution/stack.hpp"

#define INSTRUCTION_INVOKE_FUNCTION [[maybe_unused]] Stack& stack

namespace jvm {

using Bytes = std::span<std::uint8_t>;

namespace op {

struct nop;
struct aconst_null;
struct iconst_m1;
struct iconst_0;
struct iconst_1;
struct iconst_2;
struct iconst_3;
struct iconst_4;
struct iconst_5;

struct aload;

struct aload_0;
struct aload_1;
struct aload_2;
struct aload_3;

struct ret;

struct invokespectial;
}

}
