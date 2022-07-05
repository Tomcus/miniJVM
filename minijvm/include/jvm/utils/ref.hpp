#pragma once

namespace jvm {

template<typename Type>
class ConstRef {
public:
    ConstRef(): ptr(nullptr) { }
    ConstRef(const Type& other): ptr(&const_cast<Type &>(other)) { }
    ConstRef(ConstRef<Type>& other): ptr(other.ptr) { }
    ConstRef(ConstRef<Type>&& other): ptr(other.ptr) { }

    inline ConstRef<Type>& operator=(const Type& other) {
        ptr = &const_cast<Type&>(other);
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

template<typename Type>
class Ref: public ConstRef<Type> {
public:
    Ref(): ConstRef<Type>() { }
    Ref(Type& other): ConstRef<Type>(other) { }
    Ref(Ref<Type>& other): ConstRef<Type>(other)  { }
    Ref(Ref<Type>&& other): ConstRef<Type>(other)  { }
    
    inline Ref<Type>& operator=(const Type& other) {
        this->ptr = &const_cast<Type&>(other);
        return *this;
    }

    inline Ref<Type>& operator=(Ref<Type>& other) {
        this->ptr = other.ptr;
        return *this;
    }

    inline Ref<Type>& operator=(Ref<Type>&& other) {
        this->ptr = other.ptr;
        return *this;
    }

    inline Type* operator->() {
        return this->ptr;
    }

    inline Type* operator*() {
        return this->ptr;
    }

    inline operator Type&() {
        return *this->ptr;
    }
};
}
