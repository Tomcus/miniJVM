#include "types.hpp"

#include <exception>
#include <filesystem>
#include <optional>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <lyra/lyra.hpp>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#pragma GCC diagnostic pop

template <typename Type>
std::string dumpBasicTypeValue(const Type& /*value*/) {
    return fmt::format("Unsupported basic type: {}", typeid(Type).name());
}

template <>
std::string dumpBasicTypeValue(const std::string& value) {
    return fmt::format("String<\"{}\">", value);
}

template <typename Type>
std::string dumpConstPoolValue(const Type& /*value*/) {
    return fmt::format("Unsupported const pool type: {}", typeid(Type).name());
}

template <>
std::string dumpConstPoolValue(const jvm::BasicType& value) {
    return std::visit([](const auto& val){
        return dumpBasicTypeValue(val);
    }, value);
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
std::string dumpConstPoolValue(const jvm::ConstPool::NameAndType& value) {
    return fmt::format("NameAndType<name: {}, type: {}>", value.name, value.type);
}

void dumpConstPool(const std::vector<jvm::ConstPool::Value>& constPool, std::ostream& output) {
    for (std::size_t i = 0; i < constPool.size(); ++i) {
        output << "\t" << i << ": ";
        output << std::visit([](const auto& val) {
            return dumpConstPoolValue(val);
        }, constPool[i]);
        output << "\n";
    }
}

void dumpInterfaces(const jvm::Interfaces& interfaces, std::ostream& output) {
    for (const auto& interface: interfaces) {
        output << "\tInterface<index: " << interface << ">\n";
    }
}

void dumpFields(const jvm::Fields& fields, std::ostream& output) {
    for (const auto& field: fields) {
        output << fmt::format("\tField<flags: {:#04x}, name: {}, descriptor: {}>\n", field.flags, field.nameIndex, field.descriptorIndex);
        for (const auto& attribute: field.attributes) {
            output << fmt::format("\t\tAttribute<name: {}, data: [{}]>\n", attribute.attrNameIndex, fmt::join(attribute.data, ", "));
        }
    }
}

void dumpAttribute(const jvm::Attributes& attributes, std::ostream& output) {
    for (const auto& attribute: attributes) {
        output << fmt::format("\tAttribute<name: {}, data: [{}]>\n", attribute.attrNameIndex, fmt::join(attribute.data, ", "));
    }
}

void dumpClassFile(const std::filesystem::path& pathToClass, std::ostream& output = std::cout) {
    auto classFile = jvm::Class::load(pathToClass);
    output << "Class " << classFile.getClassName() << " extends " << classFile.getParentClassName() << "\n";
    output << "v." << classFile.version.major << "." << classFile.version.minor << "\n";
    output << "======================\n";
    output << "ConstPool[" << classFile.constPool.size() << "]:\n";
    dumpConstPool(classFile.constPool, output);
    output << "Interfaces[" << classFile.interfaces.size() << "]:\n";
    dumpInterfaces(classFile.interfaces, output);
    output << "Fields[" << classFile.fields.size() << "]:\n";
    dumpFields(classFile.fields, output);
    output << "Methods[" << classFile.methods.size() << "]:\n";
    dumpFields(classFile.methods, output);
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
        spdlog::error("Unable to parse command line arguments. {}", res.message());
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
        spdlog::error("Exception occurred: {}", exc.what());
        return -2;
    }

    return 0;
}