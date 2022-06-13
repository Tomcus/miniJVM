#include <catch2/catch_test_macros.hpp>

#include "jvm/types/class/attribute.hpp"

TEST_CASE("Test if vector of Attributes can be copied") {
    std::string name1 = "foo";
    std::string name2 = "bar";
    std::string name3 = "baz";
    jvm::Attributes attrs = {
        jvm::Attribute{name1, std::vector<jvm::Byte>{0x00, 0x01, 0x02}},
        jvm::Attribute{name2, std::vector<jvm::Byte>{0x69, 0x42}},
        jvm::Attribute{name3, std::vector<jvm::Byte>{0xca, 0xfe}}
    };

    jvm::Attributes copy = attrs;
    name2 = "fig";
    attrs[0].data.emplace_back(0x12);

    REQUIRE(*copy[1].name == &name2);
    REQUIRE(copy[0].data.size() == 3);
}