#include <boost/test/unit_test.hpp>

#include <xmlrpc/xmlrpc.hpp>

#include <type_traits>

using namespace xmlrpc;

BOOST_AUTO_TEST_SUITE(xmlrpc_call)

BOOST_AUTO_TEST_CASE(NoArguments) {
    MethodArgs method("method");
    auto vals = method.call();
    BOOST_CHECK_EQUAL(vals.size(), 0);
}

BOOST_AUTO_TEST_CASE(OneNullArgument) {
    MethodArgs method("method");
    auto vals = method.call(nullptr);
    BOOST_CHECK(vals.size() == 1);
    BOOST_CHECK(std::holds_alternative<Nil>(vals[0].variant()));
}

BOOST_AUTO_TEST_CASE(OneIntArgument) {
    MethodArgs method("method");
    auto vals = method.call(987);
    BOOST_CHECK(vals.size() == 1);
    BOOST_REQUIRE(std::holds_alternative<Int>(vals[0].variant()));
    BOOST_CHECK_EQUAL(get<Int>(vals[0].variant()).value(), 987);
}

BOOST_AUTO_TEST_CASE(OneDoubleArgument) {
    MethodArgs method("method");
    auto vals = method.call(4.125);
    BOOST_CHECK(vals.size() == 1);
    BOOST_REQUIRE(std::holds_alternative<Double>(vals[0].variant()));
    BOOST_CHECK_EQUAL(get<Double>(vals[0].variant()).value(), 4.125);
}

BOOST_AUTO_TEST_CASE(OneStringArgument) {
    MethodArgs method("method");
    auto vals = method.call("arg");
    BOOST_CHECK(vals.size() == 1);
    BOOST_REQUIRE(std::holds_alternative<String>(vals[0].variant()));
    BOOST_CHECK_EQUAL(get<String>(vals[0].variant()).value(), "arg");
}

BOOST_AUTO_TEST_CASE(OneArrayArgument) {
    MethodArgs method("method");
    Array a;
    a.emplace_back(1);
    auto vals = method.call(a);
    BOOST_CHECK(vals.size() == 1);
    BOOST_REQUIRE(std::holds_alternative<Array>(vals[0].variant()));
    auto &a2 = get<Array>(vals[0].variant());
    BOOST_REQUIRE_EQUAL(a2.size(), 1);
    BOOST_REQUIRE(std::holds_alternative<Int>(a2[0].variant()));
}

BOOST_AUTO_TEST_CASE(OneStructArgument) {
    MethodArgs method("method");
    Struct s;
    s.emplace("a0", 3);
    auto vals = method.call(s);
    BOOST_CHECK(vals.size() == 1);
    BOOST_REQUIRE(std::holds_alternative<Struct>(vals[0].variant()));
    auto &s2 = get<Struct>(vals[0].variant());
    BOOST_REQUIRE_EQUAL(s2.size(), 1);
    BOOST_CHECK_EQUAL(s2.get<Int>("a0").value(), 3);
}

BOOST_AUTO_TEST_CASE(FourArguments) {
    MethodArgs method("method");
    auto vals = method.call(1, 4, 16, 64, 256);
    BOOST_CHECK_EQUAL(vals.size(), 5);
    int i = 1;
    for (auto & v : vals) {
        BOOST_REQUIRE(holds_alternative<Int>(v));
        auto vi = get<Int>(v.variant()).value();
        BOOST_CHECK_EQUAL(vi, i);
        i *= 4;
    }
}

BOOST_AUTO_TEST_SUITE_END()
