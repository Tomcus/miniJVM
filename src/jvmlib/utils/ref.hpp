#ifndef MINI_JVM_LIB_UTILS_REF_HPP
#define MINI_JVM_LIB_UTILS_REF_HPP

namespace jvm {

template<typename Type>
struct ConstRef {
    ConstRef(): ptr(nullptr) { }
    ConstRef(Type& other): ptr(&other) { }
    ConstRef(ConstRef<Type>& other): ptr(other.ptr) { }
    ConstRef(ConstRef<Type>&& other): ptr(other.ptr) { }

    inline ConstRef<Type>& operator=(Type& other) {
        ptr = &other;
        return *this;
    }

    inline ConstRef<Type>& operator=(ConstRef<Type>& other) {
        ptr = other.ptr;
        return *this;
    }

    inline ConstRef<Type>& operator=(ConstRef<Type>&& other) {
        ptr = other.ptr;
        return *this;
    }

    inline const Type* operator->() const {
        return ptr;
    }

    inline const Type* operator*() const {
        return ptr;
    }

    inline operator Type&() const {
        return *ptr;
    }

    inline operator bool() const {
        return ptr != nullptr;
    }
protected:
    Type* ptr;
};

}

#endif//MINI_JVM_LIB_UTILS_REF_HPP