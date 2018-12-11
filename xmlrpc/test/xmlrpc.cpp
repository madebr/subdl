#include <boost/test/unit_test.hpp>

#include <xmlrpc/xmlrpc.hpp>

#include <type_traits>

using namespace xmlrpc;

BOOST_AUTO_TEST_SUITE(xmlrpc_call)

BOOST_AUTO_TEST_CASE(NoArguments) {
    CallMethod method("method");
    method.call();
    BOOST_CHECK_EQUAL(method.data().size(), 0);

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneNullArgument) {
    CallMethod method("method");
    method.call(nullptr);
    BOOST_CHECK(method.data().size() == 1);
    BOOST_CHECK(holds_alternative<Nil>(method.data()[0]));

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneIntArgument) {
    CallMethod method("method");
    method.call(987);
    BOOST_CHECK(method.data().size() == 1);
    BOOST_REQUIRE(holds_alternative<Int>(method.data()[0]));
    BOOST_CHECK_EQUAL(get<Int>(method.data()[0]).value(), 987);

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneDoubleArgument) {
    CallMethod method("method");
    method.call(4.125);
    BOOST_CHECK(method.data().size() == 1);
    BOOST_REQUIRE(holds_alternative<Double>(method.data()[0]));
    BOOST_CHECK_EQUAL(get<Double>(method.data()[0]).value(), 4.125);

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneStringArgument) {
    CallMethod method("method");
    method.call("arg");
    BOOST_CHECK(method.data().size() == 1);
    BOOST_REQUIRE(holds_alternative<String>(method.data()[0]));
    BOOST_CHECK_EQUAL(get<String>(method.data()[0]).value(), "arg");

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneArrayArgument) {
    CallMethod method("method");
    Array a;
    a.emplace_back(1);
    method.call(a);
    BOOST_CHECK(method.data().size() == 1);
    BOOST_REQUIRE(holds_alternative<Array>(method.data()[0]));
    auto &a2 = get<Array>(method.data()[0]);
    BOOST_REQUIRE_EQUAL(a2.size(), 1);
    BOOST_REQUIRE(holds_alternative<Int>(a2[0]));

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(OneStructArgument) {
    CallMethod method("method");
    Struct s;
    s.emplace("a0", 3);
    method.call(s);
    BOOST_CHECK(method.data().size() == 1);
    BOOST_REQUIRE(holds_alternative<Struct>(method.data()[0]));
    auto &s2 = get<Struct>(method.data()[0]);
    BOOST_REQUIRE_EQUAL(s2.size(), 1);
    BOOST_CHECK_EQUAL(s2.get<Int>("a0").value(), 3);

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_CASE(FourArguments) {
    CallMethod method("method");
    method.call(1, 4, 16, 64, 256);
    BOOST_CHECK_EQUAL(method.data().size(), 5);
    int i = 1;
    for (auto & v : method.data()) {
        BOOST_REQUIRE(holds_alternative<Int>(v));
        auto vi = get<Int>(v).value();
        BOOST_CHECK_EQUAL(vi, i);
        i *= 4;
    }

    BOOST_CHECK_EQUAL(method.name(), "method");
}

BOOST_AUTO_TEST_SUITE_END()
