#pragma once

#include "basic.hpp"
#include <cstdint>
#include <variant>

namespace jvm::bytecode {

template<typename VariableType>
class StoreVariableFromStackAsLocal: public BaseInstruction {
public:
    StoreVariableFromStackAsLocal(std::uint16_t index): index(index) { }
    virtual ~StoreVariableFromStackAsLocal() = default;

    void execute(JVM_INSTRUCTION_INVOKE_PARAMS) final {
        auto val = stack.pop();
        if (index < local.size() && std::holds_alternative<VariableType>(val) &&
           (std::holds_alternative<VariableType>(local[index]) || std::holds_alternative<std::monostate>(local[index]))) {
            local[index] = std::get<VariableType>(val);  
        }
    }
protected:
    std::uint16_t index;
};

class istore: public StoreVariableFromStackAsLocal<std::int32_t> {
public:
    istore(std::uint16_t index): StoreVariableFromStackAsLocal<std::int32_t>(index) { }
};

class lstore: public StoreVariableFromStackAsLocal<std::int64_t> {
public:
    lstore(std::uint16_t index): StoreVariableFromStackAsLocal<std::int64_t>(index) { }
};

class fstore: public StoreVariableFromStackAsLocal<float> {
public:
    fstore(std::uint16_t index): StoreVariableFromStackAsLocal<float>(index) { }
};

class dstore: public StoreVariableFromStackAsLocal<double> {
public:
    dstore(std::uint16_t index): StoreVariableFromStackAsLocal<double>(index) { }
};

class istore_0: public istore {
public:
    istore_0(): istore(0) { }
};

class istore_1: public istore {
public:
    istore_1(): istore(1) { }
};

class istore_2: public istore {
public:
    istore_2(): istore(2) { }
};

class istore_3: public istore {
public:
    istore_3(): istore(3) { }
};

class lstore_0: public lstore {
public:
    lstore_0(): lstore(0) { }
};

class lstore_1: public lstore {
public:
    lstore_1(): lstore(1) { }
};

class lstore_2: public lstore {
public:
    lstore_2(): lstore(2) { }
};

class lstore_3: public lstore {
public:
    lstore_3(): lstore(3) { }
};

class fstore_0: public fstore {
public:
    fstore_0(): fstore(0) { }
};

class fstore_1: public fstore {
public:
    fstore_1(): fstore(1) { }
};

class fstore_2: public fstore {
public:
    fstore_2(): fstore(2) { }
};

class fstore_3: public fstore {
public:
    fstore_3(): fstore(3) { }
};
    
class dstore_0: public dstore {
public:
    dstore_0(): dstore(0) { }
};

class dstore_1: public dstore {
public:
    dstore_1(): dstore(1) { }
};

class dstore_2: public dstore {
public:
    dstore_2(): dstore(2) { }
};

class dstore_3: public dstore {
public:
    dstore_3(): dstore(3) { }
};

}
