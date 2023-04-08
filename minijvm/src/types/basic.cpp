#include "jvm/types/basic.hpp"

#include <sstream>

#include <fmt/format.h>

using namespace jvm;

inline const std::string_view getIStreamErrorString(const std::istream& in) {
    if (in.eof()) {
        return "End of file reached.";
    }
    if (in.fail()) {
        return "Recoverable error reached. Wrong data format read.";
    }
    if (in.bad()) {
        return "Unrecoverable error reached.";
    }
    return "No error";
}

ParsingError ParsingError::cant_read_from_istream(std::istream & input, std::string_view typeName) {
    return ParsingError {
        .message = fmt::format("Can't read {} at position {}: {}", typeName, input.tellg(), getIStreamErrorString(input))
    };
}

ParsingError ParsingError::unsuported_type() {
    return ParsingError {
        .message = "Unable to parse data: Unsuported type - implement parsing method"
    };
}
