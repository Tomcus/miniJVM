#ifndef MINI_JVM_LIB_UTILS_REF_HPP
#define MINI_JVM_LIB_UTILS_REF_HPP

namespace jvm {

template<typename Type>
struct Ref {
    Ref(): ptr(nullptr) { }
    Ref(Type& other): ptr(&other) { }
    Ref(Ref<Type>& other): ptr(other.ptr) { }

    inline Ref<Type>& operator=(Type& other) {
        ptr = &other;
        return *this;
    }

    inline Ref<Type>& operator=(Ref<Type>& other) {
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