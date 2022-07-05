#include <catch2/catch_test_macros.hpp>

#include "jvm/utils/ref.hpp"

#include <string>

struct TestStruct {
    int a;
    float b;
    std::string c;
};

void validate(const TestStruct& ts) {
    REQUIRE(ts.a == 42);
    REQUIRE(ts.b == -69.0f);
    REQUIRE(ts.c == "test");
}

const TestStruct& getRef(const std::vector<TestStruct>& data, const std::size_t index) {
    return data.at(index);
}

TEST_CASE("Basic const reference tests", "[ref]") {
    std::vector<TestStruct> testData = {
        TestStruct {1, 1.0f, "1"},
        TestStruct {2, 2.0f, "2"},
        TestStruct {3, 3.0f, "3"}
    };

    jvm::ConstRef<TestStruct> ref = testData[1];
    REQUIRE(ref->a == testData[1].a);
    REQUIRE(ref->a == 2);
    REQUIRE(ref->b == testData[1].b);
    REQUIRE(ref->b == 2.0f);
    REQUIRE(ref->c == testData[1].c);
    REQUIRE(ref->c == "2");

    testData[1].a = 42;
    REQUIRE(ref->a == 42);
    testData[1].b = -69.0f;
    REQUIRE(ref->b == -69.0f);
    testData[1].c = "test";
    REQUIRE(ref->c == "test");

    validate(ref);

    jvm::ConstRef<TestStruct> other = ref;
    validate(other);

    ref = getRef(testData, 0);
    REQUIRE(ref->a == 1);
    REQUIRE(ref->b == 1.0f);
    REQUIRE(ref->c == "1");

    REQUIRE(sizeof(ref) == 8);
}

TEST_CASE("Basic non const reference tests", "[ref]") {
    TestStruct testData{1, 2.0f, "three"};
    jvm::Ref<TestStruct> ref = testData;
    ref->a = 42;
    ref->b = -69.0f;
    ref->c = "test";

    validate(ref);
}

