#include "jvm/types.hpp"
#include "jvm/types/basic.hpp"
#include "jvm/types/const_pool.hpp"

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

#include <nonstd/expected.hpp>
#include <fmtlog/fmtlog.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <lyra/lyra.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/core.h>
#pragma GCC diagnostic pop

struct Error {
    std::string message;
    static Error from(jvm::ConstPool::Error e) {
        return Error {
            .message = std::move(e.message)
        };
    }

    static Error from(jvm::ParsingError e) {
        return Error {
            .message = std::move(e.message)
        };
    }

    static Error from(std::variant<jvm::ParsingError, jvm::ConstPool::Error> e) {
        return std::visit([](auto inner) {
            return Error::from(inner);
        }, e);
    }
};

#define CONVERT_ERROR(expected) if (!expected) nonstd::make_unexpected(Error::from(expected.error()))

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

[[nodiscard]] std::string to_string(jvm::ConstPool::MethodHandle::Type value) {
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
    return "WTF - NOT POSSIBLE";
}

template <>
std::string dumpConstPoolValue(const jvm::ConstPool::MethodHandle& value) {
    return fmt::format("MethhodHandle<kind: {}, referenceIndex: {}>", to_string(value.kind), value.referenceIndex);
}

[[nodiscard]] nonstd::expected<void, Error> dumpConstPool(const jvm::ConstPool& constPool, std::ostream& output) {
    for (std::size_t i = 1; i < constPool.size() + 1; ++i) {
        output << "\t" << i << ": ";
        auto data = constPool[i];
        if (!data) return nonstd::make_unexpected(Error::from(data.error()));
        output << std::visit([](const auto& val) {
            return dumpConstPoolValue(val);
        }, ***data);
        output << "\n";
    }
    return {};
}

void dumpInterfaces(const jvm::ConstPool& cp, const jvm::Interfaces& interfaces, std::ostream& output) {
    if (!interfaces.empty()) {
        output << "implements ";
        std::vector<std::string> interfacesStrings;
        std::optional<Error> err;
        std::transform(interfaces.cbegin(), interfaces.cend(), std::back_inserter(interfacesStrings),
            [&](const auto& interface) -> std::string {
                auto data = cp.getRef<std::string>(interface->name);
                if (!data) {
                    if (!err.has_value()) {
                        err = Error::from(data.error());
                    }
                    return "";
                }
                return ***data;
            }
        );
        output << fmt::format("{}", fmt::join(interfacesStrings, ", "));
        output << '\n';
    }
}

void dumpFields(const jvm::Fields& fields, std::ostream& output) {
    for (const auto& field: fields) {
        output << fmt::format("\tField<flags: {:#04x}, name: {}, descriptor: {}>\n", static_cast<std::uint16_t>(field.flags), std::string(field.name), std::string(field.typeDescriptor));
        for (const auto& attribute: field.attributes) {
            output << fmt::format("\t\tAttribute<name: {}, data: [{}]>\n", std::string(attribute.name), fmt::join(attribute.data, ", "));
        }
    }
}

template<typename InstructionType>
void dumpInstruction(std::ostream& output, const InstructionType&) {
    output << "UNKNOWN INSTRUCTION";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::nop&) {
    output << "nop";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aconst_null&) {
    output << "aconst_null";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_m1&) {
    output << "iconst_m1";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_0&) {
    output << "iconst_0";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_1&) {
    output << "iconst_1";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_2&) {
    output << "iconst_2";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_3&) {
    output << "iconst_3";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_4&) {
    output << "iconst_4";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iconst_5&) {
    output << "iconst_5";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::lconst_0&) {
    output << "lconst_0";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::lconst_1&) {
    output << "lconst_1";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aload_0&) {
    output << "aload_0";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aload_1&) {
    output << "aload_1";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aload_2&) {
    output << "aload_2";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aload_3&) {
    output << "aload_3";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iload_0&) {
    output << "iload_0";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iload_1&) {
    output << "iload_1";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iload_2&) {
    output << "iload_2";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iload_3&) {
    output << "iload_3";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::aload& ins) {
    output << fmt::format("aload<{}>", ins.localIndex);
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iload& ins) {
    output << fmt::format("iload<{}>", ins.localIndex);
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::ret&) {
    output << "return";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::ireturn&) {
    output << "ireturn";
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::invokespecial& ins) {
    output << fmt::format("invokespecial<{}>", ins.cpIndex);
}

template<>
void dumpInstruction(std::ostream& output, const jvm::op::iadd&) {
    output << "iadd";
}

void dumpCode(std::ostream& output, const jvm::Instructions& code) {
    bool first = true;
    output << fmt::format("\t\tCode: ");
    for (const auto& instruction: code) {
        if (first) {
            first = false;
        } else {
            output << ", ";
        }
        std::visit([&](auto& ins) {
            dumpInstruction(output, ins);
        }, instruction);
    }
    output << '\n';
}

void dumpMethods(const jvm::Methods& methods, std::ostream& output) {
    for (const auto& method: methods) {
        output << fmt::format("\tMethod<flags: {:#04x}, name: {}, descriptor: {}>\n", static_cast<std::uint16_t>(method.flags), std::string(method.name), std::string(method.typeDescriptor));
        dumpCode(output, method.code);
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

[[nodiscard]] nonstd::expected<void, Error> dumpClassFile(const std::filesystem::path& pathToClass, std::ostream& output = std::cout) {
    auto classFile = jvm::Class::load(pathToClass);
    CONVERT_ERROR(classFile);
    auto className = classFile->getClassName();
    CONVERT_ERROR(className);
    auto superName = classFile->getParentClassName();
    CONVERT_ERROR(superName);
    output << "Class " << *className << " extends " << *superName << "\n";
    dumpInterfaces(classFile->constPool, classFile->interfaces, output);
    output << "v." << classFile->version.major << "." << classFile->version.minor << "\n";
    output << "======================\n";
    output << "ConstPool[" << classFile->constPool.size() << "]:\n";
    auto res = dumpConstPool(classFile->constPool, output);
    PROPAGATE_ERROR(res);
    output << "Fields[" << classFile->fields.size() << "]:\n";
    dumpFields(classFile->fields, output);
    output << "Methods[" << classFile->methods.size() << "]:\n";
    dumpMethods(classFile->methods, output);
    output << "Attributes[" << classFile->attributes.size() << "]:\n";
    dumpAttribute(classFile->attributes, output);
    return {};
}

void setLogLevel(std::string_view level) {
    if (level == "debug" || level == "dbg") {
        fmtlog::setLogLevel(fmtlog::DBG);
    }
}

template<> struct fmt::formatter<lyra::cli>: fmt::ostream_formatter { };

int main(int argc, const char* argv[]) {
    std::filesystem::path classFile;
    std::optional<std::filesystem::path> outputFile;
    std::optional<std::string> logLevel;
    bool showHelp = false;
    auto cli = lyra::cli()
        | lyra::help(showHelp)
        | lyra::arg(classFile, "Class File") ("which class file to dump")
        | lyra::opt(outputFile, "Output file")["-o"]["--output"]("output file where to create the dump")
        | lyra::opt(logLevel, "Log level")["-l"]["--log-level"]("output log level");
    auto res = cli.parse({argc, argv});
    if (!res) {
        fmt::print("Unable to parse command line arguments. {}", res.message());
        return -1;
    }

    if (showHelp) {
        fmt::print("{}\n", cli);
        return 0;
    }

    if (logLevel) {
        setLogLevel(*logLevel);
    }

    if (outputFile) {
        auto ostream = std::ofstream(*outputFile);
        auto dumpRes = dumpClassFile(classFile, ostream);
        if (!dumpRes) {
            fmt::print("Unable to load and parse class: {}", dumpRes.error().message);
            fmtlog::poll();
            return -2;
        }
    } else {
        auto dumpRes = dumpClassFile(classFile, std::cout);
        if (!dumpRes) {
            fmt::print("Unable to load and parse class: {}", dumpRes.error().message);
            fmtlog::poll();
            return -2;
        }
    }

    fmtlog::poll();
    return 0;
}
