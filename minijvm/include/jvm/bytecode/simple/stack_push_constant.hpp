#pragma once

#include "basic.hpp"
#include "jvm/bytecode/simple/forward.hpp"

namespace jvm::bytecode {

template<typename Type, std::int64_t val>
class BaseStackPushNumericConstant: public BaseInstruction {
public:
    BaseStackPushNumericConstant() = default;
    virtual ~BaseStackPushNumericConstant() = default;

    void execute(JVM_INSTRUCTION_INVOKE_PARAMS) final {
        stack.push(static_cast<Type>(val));
    }
};
 
class iconst_m1 : public BaseStackPushNumericConstant<std::int32_t, -1>{};
class iconst_0: public BaseStackPushNumericConstant<std::int32_t, 0>{};
class iconst_1: public BaseStackPushNumericConstant<std::int32_t, 1>{};
class iconst_2: public BaseStackPushNumericConstant<std::int32_t, 2>{};
class iconst_3: public BaseStackPushNumericConstant<std::int32_t, 3>{};
class iconst_4: public BaseStackPushNumericConstant<std::int32_t, 4>{};
class iconst_5: public BaseStackPushNumericConstant<std::int32_t, 5>{};

class lconst_0: public BaseStackPushNumericConstant<std::int64_t, 0>{};
class lconst_1: public BaseStackPushNumericConstant<std::int64_t, 1>{};

class fconst_0: public BaseStackPushNumericConstant<float, 0>{};
class fconst_1: public BaseStackPushNumericConstant<float, 1>{};
class fconst_2: public BaseStackPushNumericConstant<float, 2>{};

class dconst_0: public BaseStackPushNumericConstant<double, 0>{};
class dconst_1: public BaseStackPushNumericConstant<double, 1>{};

class aconst_null: public BaseInstruction {
public:
    aconst_null() = default;
    virtual ~aconst_null() = default;

    void execute(JVM_INSTRUCTION_INVOKE_PARAMS) final;
};
}
