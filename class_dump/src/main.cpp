#include "jvm/types.hpp"

#include <algorithm>
#include <exception>
#include <filesystem>
#include <iterator>
#include <optional>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <lyra/lyra.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#pragma GCC diagnostic pop

template <typename Type>
std::string dumpConstPoolValue(const Type& /*value*/) {
    return fmt::format("Unsupported const pool type: {}", typeid(Type).name());
}

template <>
std::string dumpConstPoolValue(const std::string& value) {
    return fmt::format("String<\"{}\">", value);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::ClassInfo& value) {
    return fmt::format("ClassInfo<name: {}>", value.name);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::MethodRef& value) {
    return fmt::format("MethodRef<classInfo: {}, nameAndType: {}>", value.classInfo, value.nameAndType);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::FieldRef& value) {
    return fmt::format("FieldRef<classInfo: {}, nameAndType: {}>", value.classInfo, value.nameAndType);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::InvokeDynamic& value) {
    return fmt::format("InvokeDynamic<bootstrapMethod: {}, nameAndType: {}>", value.boostrapMethod, value.nameAndType);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::StringRef& value) {
    return fmt::format("StringRef<stringIndex: {}>", value.string);
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::NameAndType& value) {
    return fmt::format("NameAndType<name: {}, type: {}>", value.name, value.type);
}

std::string to_string(jvm::ConstPool::MethodHandle::Type value) {
    switch (value) {
        case jvm::ConstPool::MethodHandle::Type::GET_FIELD:
            return "GET_FIELD";
        case jvm::ConstPool::MethodHandle::Type::PUT_FIELD:
            return "PUT_FIELD";
        case jvm::ConstPool::MethodHandle::Type::GET_STATIC:
            return "GET_STATIC";
        case jvm::ConstPool::MethodHandle::Type::PUT_STATIC:
            return "PUT_STATIC";
        case jvm::ConstPool::MethodHandle::Type::INVOKE_STATIC:
            return "INVOKE_STATIC";
        case jvm::ConstPool::MethodHandle::Type::INVOKE_INTERFACE:
            return "INVOKE_INTERFACE";
        case jvm::ConstPool::MethodHandle::Type::INVOKE_SPECIAL:
            return "INVOKE_SPECIAL";
        case jvm::ConstPool::MethodHandle::Type::INVOKE_VIRTUAL:
            return "INVOKE_VIRTUAL";
        case jvm::ConstPool::MethodHandle::Type::NEW_INVOKE_SPECIAL:
            return "NEW_INVOKE_SPECIAL";
    }
    throw std::runtime_error("WTF - This is imposible");
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::MethodHandle& value) {
    return fmt::format("MethhodHandle<kind: {}, referenceIndex: {}>", to_string(value.kind), value.referenceIndex);
}

void dumpConstPool(const jvm::ConstPool& constPool, std::ostream& output) {
    for (std::size_t i = 1; i < constPool.size() + 1; ++i) {
        output << "\t" << i << ": ";
        output << std::visit([](const auto& val) {
            return dumpConstPoolValue(val);
        }, constPool[i]);
        output << "\n";
    }
}

void dumpInterfaces(const jvm::ConstPool& cp, const jvm::Interfaces& interfaces, std::ostream& output) {
    if (!interfaces.empty()) {
        output << "implements ";
        std::vector<std::string> interfacesStrings;
        std::transform(interfaces.cbegin(), interfaces.cend(), std::back_inserter(interfacesStrings), [&](const auto& interface) {
            return cp.getRef<std::string>(interface->name);
        });
        output << fmt::format("{}", fmt::join(interfacesStrings, ", "));
        output << '\n';
    }
}

void dumpFields(const jvm::Fields& fields, std::ostream& output) {
    for (const auto& field: fields) {
        output << fmt::format("\tField<flags: {:#04x}, name: {}, descriptor: {}>\n", static_cast<std::uint16_t>(field.flags), std::string(field.name), field.descriptorIndex);
        for (const auto& attribute: field.attributes) {
            output << fmt::format("\t\tAttribute<name: {}, data: [{}]>\n", std::string(attribute.name), fmt::join(attribute.data, ", "));
        }
    }
}

void dumpMethods(const jvm::Methods& methods, std::ostream& output) {
    for (const auto& method: methods) {
        output << fmt::format("\tMethod<flags: {:#04x}, name: {}, descriptor: {}>\n", method.flags, std::string(method.name), method.descriptorIndex);
        for (const auto& attribute: method.attributes) {
            output << fmt::format("\t\tAttribute<name: {}, data: [{}]>\n", std::string(attribute.name), fmt::join(attribute.data, ", "));
        }
    }
}

void dumpAttribute(const jvm::Attributes& attributes, std::ostream& output) {
    for (const auto& attribute: attributes) {
        output << fmt::format("\tAttribute<name: {}, data: [{}]>\n", std::string(attribute.name), fmt::join(attribute.data, ", "));
    }
}

void dumpClassFile(const std::filesystem::path& pathToClass, std::ostream& output = std::cout) {
    auto classFile = jvm::Class::load(pathToClass);
    output << "Class " << classFile.getClassName() << " extends " << classFile.getParentClassName() << "\n";
    dumpInterfaces(classFile.constPool, classFile.interfaces, output);
    output << "v." << classFile.version.major << "." << classFile.version.minor << "\n";
    output << "======================\n";
    output << "ConstPool[" << classFile.constPool.size() << "]:\n";
    dumpConstPool(classFile.constPool, output);
    output << "Fields[" << classFile.fields.size() << "]:\n";
    dumpFields(classFile.fields, output);
    output << "Methods[" << classFile.methods.size() << "]:\n";
    dumpMethods(classFile.methods, output);
    output << "Attributes[" << classFile.attributes.size() << "]:\n";
    dumpAttribute(classFile.attributes, output);
}

int main(int argc, const char* argv[]) {
    std::filesystem::path classFile;
    std::optional<std::filesystem::path> outputFile;
    auto cli = lyra::cli()
        | lyra::arg(classFile, "Class File") ("which class file to dump")
        | lyra::opt(outputFile, "Output file")["-o"]["--output"]("output file where to create the dump");
    auto res = cli.parse({argc, argv});
    if (!res) {
        fmt::print("Unable to parse command line arguments. {}", res.message());
        return -1;
    }

    try {
        if (outputFile) {
            auto ostream = std::ofstream(*outputFile);
            dumpClassFile(classFile, ostream);
        } else {
            dumpClassFile(classFile, std::cout);
        }
    } catch (std::exception& exc) {
        fmt::print("Exception occurred: {}", exc.what());
        return -2;
    }

    return 0;
}
