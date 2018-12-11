#include <boost/test/unit_test.hpp>

#include <xmlrpc/types.hpp>

#include <type_traits>

using namespace xmlrpc;

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Nil)

BOOST_AUTO_TEST_CASE(Base) {
    Nil nil;
    const bool check = std::is_same<decltype(nil)::value_type, nullptr_t>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Nil nil;
        Nil nil2{nil};
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Boolean)

BOOST_AUTO_TEST_CASE(Base) {
    Boolean b;
    const bool check = std::is_same<decltype(b)::value_type, bool>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Boolean b;
        BOOST_CHECK_EQUAL(b.value(), false);
        Boolean b2{b};
        BOOST_CHECK_EQUAL(b.value(), false);
        BOOST_CHECK_EQUAL(b2.value(), false);
    }
    {
        Boolean b{false};
        BOOST_CHECK_EQUAL(b.value(), false);
        Boolean b2{b};
        BOOST_CHECK_EQUAL(b.value(), false);
        BOOST_CHECK_EQUAL(b2.value(), false);
    }
    {
        Boolean b{true};
        BOOST_CHECK_EQUAL(b.value(), true);
        Boolean b2{b};
        BOOST_CHECK_EQUAL(b.value(), true);
        BOOST_CHECK_EQUAL(b2.value(), true);
    }
}

BOOST_AUTO_TEST_CASE(SetGet) {
    Boolean b;
    BOOST_CHECK_EQUAL(b.value(), false);
    b.set_value(true);
    BOOST_CHECK_EQUAL(b.value(), true);
    b.set_value(false);
    BOOST_CHECK_EQUAL(b.value(), false);
    b.set_value(false);
    BOOST_CHECK_EQUAL(b.value(), false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Int)

BOOST_AUTO_TEST_CASE(Base) {
    Int i;
    const bool check = std::is_same<decltype(i)::value_type, int>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Int i;
        BOOST_CHECK_EQUAL(i.value(), 0);
        Int i2{i};
        BOOST_CHECK_EQUAL(i.value(), 0);
        BOOST_CHECK_EQUAL(i2.value(), 0);
    }
    {
        Int i{-5};
        BOOST_CHECK_EQUAL(i.value(), -5);
        Int i2{i};
        BOOST_CHECK_EQUAL(i.value(), -5);
        BOOST_CHECK_EQUAL(i2.value(), -5);
    }
    {
        Int i{5};
        BOOST_CHECK_EQUAL(i.value(), 5);
        Int i2{i};
        BOOST_CHECK_EQUAL(i.value(), 5);
        BOOST_CHECK_EQUAL(i2.value(), 5);
    }
}

BOOST_AUTO_TEST_CASE(SetGet) {
    Int i;
    BOOST_CHECK_EQUAL(i.value(), 0);
    i.set_value(-5);
    BOOST_CHECK_EQUAL(i.value(), -5);
    i.set_value(5);
    BOOST_CHECK_EQUAL(i.value(), 5);
    i.set_value(5);
    BOOST_CHECK_EQUAL(i.value(), 5);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Double)

BOOST_AUTO_TEST_CASE(Base) {
    Double d;
    const bool check = std::is_same<decltype(d)::value_type, double>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Double d;
        BOOST_CHECK_EQUAL(d.value(), 0.);
        Double d2{d};
        BOOST_CHECK_EQUAL(d.value(), 0.);
        BOOST_CHECK_EQUAL(d2.value(), 0.);
    }
    {
        Double d{1.34};
        BOOST_CHECK_EQUAL(d.value(), 1.34);
        Double d2{d};
        BOOST_CHECK_EQUAL(d.value(), 1.34);
        BOOST_CHECK_EQUAL(d2.value(), 1.34);
    }
    {
        Double d{1.5e25};
        BOOST_CHECK_EQUAL(d.value(), 1.5e25);
        Double d2{d};
        BOOST_CHECK_EQUAL(d.value(), 1.5e25);
        BOOST_CHECK_EQUAL(d2.value(), 1.5e25);
    }
}

BOOST_AUTO_TEST_CASE(SetGet) {
    Double d;
    BOOST_CHECK_EQUAL(d.value(), 0.);
    d.set_value(3.145);
    BOOST_CHECK_EQUAL(d.value(), 3.145);
    d.set_value(-9.45);
    BOOST_CHECK_EQUAL(d.value(), -9.45);
    d.set_value(-9.45);
    BOOST_CHECK_EQUAL(d.value(), -9.45);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_String)

BOOST_AUTO_TEST_CASE(Base) {
    String s;
    const bool check = std::is_same<decltype(s)::value_type, std::string>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        String s;
        BOOST_CHECK_EQUAL(s.value(), std::string{""});
        String s2{s};
        BOOST_CHECK_EQUAL(s.value(), std::string{""});
        BOOST_CHECK_EQUAL(s2.value(), std::string{""});
    }
    {
        String s{"some very long text"};
        BOOST_CHECK_EQUAL(s.value(), std::string{"some very long text"});
        String s2{s};
        BOOST_CHECK_EQUAL(s.value(), std::string{"some very long text"});
        BOOST_CHECK_EQUAL(s2.value(), std::string{"some very long text"});
    }
    {
        String s{std::string{"some very long text"}};
        BOOST_CHECK_EQUAL(s.value(), std::string{"some very long text"});
    }
}

BOOST_AUTO_TEST_CASE(SetGet) {
    String s;
    BOOST_CHECK_EQUAL(s.value(), std::string{""});
    s.set_value("abc");
    BOOST_CHECK_EQUAL(s.value(), std::string{"abc"});
    s.set_value("a long story");
    BOOST_CHECK_EQUAL(s.value(), std::string{"a long story"});
    const auto &v = s.value();
    s.set_value("a long story");
    BOOST_CHECK_EQUAL(s.value(), "a long story");
    BOOST_CHECK(&v == &s.value());

    std::string my = "mystring";
    s.set_value(my);
    BOOST_CHECK_EQUAL(s.value(), "mystring");
    BOOST_CHECK_EQUAL(my, "mystring");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Array)

BOOST_AUTO_TEST_CASE(Base) {
    Array a;
    const bool check = std::is_same<decltype(a)::value_type, std::vector<Value>>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
    }
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(Nil{});
        a.push_back(Nil{});
        BOOST_CHECK_EQUAL(a.size(), 2);
        Array a2{a};
        BOOST_CHECK_EQUAL(a2.size(), 2);
    }
}

BOOST_AUTO_TEST_CASE(PushBackNil) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(Nil{});
        BOOST_CHECK_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Nil>(g));
        auto &n1 = get<Nil>(g);
        auto &n2 = a.get<Nil>(0);
    }
    {
        Array a;
        Nil nil;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(nil);
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Nil>(g));
        auto &n1 = get<Nil>(g);
        auto &n2 = a.get<Nil>(0);
    }
    {
        Array a;
        Nil nil;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(nil);
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        a.push_back(nil);
        BOOST_REQUIRE_EQUAL(a.size(), 2);
        const auto &e0 = a[0];
        const auto &e1 = a[1];
        BOOST_REQUIRE(holds_alternative<Nil>(e0));
        BOOST_CHECK(holds_alternative<Nil>(e1));
        BOOST_CHECK_NE(&e0, &e1);
        auto &n1 = get<Nil>(e0);
        auto &n2 = a.get<Nil>(0);
    }
}

BOOST_AUTO_TEST_CASE(EmplaceBackNil) {
    Array a;
    BOOST_CHECK_EQUAL(a.size(), 0);
    a.emplace_back();
    const auto &g = a[0];
    BOOST_REQUIRE(holds_alternative<Nil>(g));
    auto &n1 = get<Nil>(g);
    auto &n2 = a.get<Nil>(0);
}

BOOST_AUTO_TEST_CASE(PushBackInt) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(Int{42});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Int>(g));
        BOOST_CHECK_EQUAL(get<Int>(g).value(), 42);
        BOOST_CHECK_EQUAL(a.get<Int>(0).value(), 42);
    }
    {
        Array a;
        Int i{42};
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(i);
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Int>(g));
        const auto &i2 = get<Int>(g);
        BOOST_CHECK_EQUAL(i2.value(), 42);
        i.set_value(82);
        BOOST_CHECK_EQUAL(i2.value(), 42);
        BOOST_CHECK_EQUAL(a.get<Int>(0).value(), 42);
    }
}

BOOST_AUTO_TEST_CASE(EmplaceBackInt) {
    Array a;
    BOOST_CHECK_EQUAL(a.size(), 0);
    a.emplace_back(4);
    const auto &g = a[0];
    BOOST_REQUIRE(holds_alternative<Int>(g));
    auto i = get<Int>(g);
    BOOST_CHECK_EQUAL(i.value(), 4);
}

BOOST_AUTO_TEST_CASE(PushBackDouble) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(Double{1.54});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Double>(g));
        BOOST_CHECK_EQUAL(get<Double>(g).value(), 1.54);
        BOOST_CHECK_EQUAL(a.get<Double>(0).value(), 1.54);
    }
    {
        Array a;
        Double d{3.14159};
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(d);
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Double>(g));
        const auto &d2 = get<Double>(g);
        BOOST_CHECK_EQUAL(d2.value(), 3.14159);
        d.set_value(2.87);
        BOOST_CHECK_EQUAL(d2.value(), 3.14159);
        BOOST_CHECK_EQUAL(a.get<Double>(0).value(), 3.14159);
    }
}

BOOST_AUTO_TEST_CASE(EmplaceBackDouble) {
    Array a;
    BOOST_CHECK_EQUAL(a.size(), 0);
    a.emplace_back(4.5);
    const auto &g = a[0];
    BOOST_REQUIRE(holds_alternative<Double>(g));
    auto d = get<Double>(g);
    BOOST_CHECK_EQUAL(d.value(), 4.5);
}

BOOST_AUTO_TEST_CASE(PushBackString) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(String{"hello string"});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<String>(g));
        BOOST_CHECK_EQUAL(get<String>(g).value(), "hello string");
        BOOST_CHECK_EQUAL(a.get<String>(0).value(), "hello string");
    }
    {
        Array a;
        String s{"long string"};
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(s);
        BOOST_CHECK_EQUAL(s.value(), "long string");
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<String>(g));
        const auto &s2 = get<String>(g);
        BOOST_CHECK_EQUAL(s2.value(), "long string");
        s.value() += " even longer";
        BOOST_CHECK_EQUAL(s.value(), "long string even longer");
        BOOST_CHECK_EQUAL(s2.value(), "long string");
        BOOST_CHECK_EQUAL(a.get<String>(0).value(), "long string");
    }
}

BOOST_AUTO_TEST_CASE(EmplaceBackString) {
    Array a;
    BOOST_CHECK_EQUAL(a.size(), 0);
    a.emplace_back("a nice string");
    const auto &g = a[0];
    BOOST_REQUIRE(holds_alternative<String>(g));
    auto s = get<String>(g);
    BOOST_CHECK_EQUAL(s.value(), "a nice string");
}

BOOST_AUTO_TEST_CASE(PushBackArray) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        auto &a_child = a.push_back(Array{});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Array>(g));
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        a_child.push_back(Nil{});
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
        BOOST_CHECK_EQUAL(a.get<Array>(0).size(), 1);
    }
    {
        Array a;
        Array a2;
        BOOST_CHECK_EQUAL(a.size(), 0);
        auto &a_child = a.push_back(Array{});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Array>(g));
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        BOOST_CHECK_EQUAL(a2.size(), 0);

        a2.push_back(Nil{});
        BOOST_CHECK_EQUAL(a2.size(), 1);
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);

        a_child.push_back(Nil{});
        BOOST_CHECK_EQUAL(a2.size(), 1);
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(EmplaceBackArray) {
    Array a;
    BOOST_CHECK_EQUAL(a.size(), 0);
    a.emplace_back("a nice string");
    const auto &g = a[0];
    BOOST_REQUIRE(holds_alternative<String>(g));
    auto s = get<String>(g);
    BOOST_CHECK_EQUAL(s.value(), "a nice string");
}

BOOST_AUTO_TEST_CASE(PushBackStruct) {
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        auto &a_child = a.push_back(Struct{});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Struct>(g));
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        a_child.insert("n0", Nil{});
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
        BOOST_CHECK_EQUAL(a.get<Struct>(0).size(), 1);
    }
    {
        Array a;
        Array a2;
        BOOST_CHECK_EQUAL(a.size(), 0);
        BOOST_CHECK_EQUAL(a2.size(), 0);
        auto &a_child = a.push_back(Struct{});
        BOOST_REQUIRE_EQUAL(a.size(), 1);
        BOOST_REQUIRE_EQUAL(a2.size(), 0);
        const auto &g = a[0];
        BOOST_REQUIRE(holds_alternative<Struct>(g));
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        BOOST_CHECK_EQUAL(a2.size(), 0);

        a2.push_back(Nil{});
        BOOST_CHECK_EQUAL(a2.size(), 1);
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);

        a_child.insert("n0", Nil{});
        BOOST_CHECK_EQUAL(a2.size(), 1);
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(ForLoop) {
    {
        Array a;
        static constexpr auto NB = 5;
        for (size_t i = 0; i < NB; ++i) {
            a.push_back(Int{1 << i});
        }
        int iteration = 0;
        for (const auto &v : a) {
            const int ref = 1 << iteration;
            BOOST_REQUIRE(holds_alternative<Int>(v));
            int i = get<Int>(v).value();
            BOOST_CHECK_EQUAL(i, ref);
            int i2 = a.get<Int>(iteration).value();
            BOOST_CHECK_EQUAL(i, i2);
            ++iteration;
        }
        BOOST_CHECK_EQUAL(iteration, NB);
    }
}

BOOST_AUTO_TEST_CASE(Iterator) {
    {
        Array a;
        a.push_back(Nil{});
        a.push_back(Boolean{true});
        a.push_back(Int{42});

        auto it = a.begin();
        auto end = a.end();

        BOOST_REQUIRE(it != end);
        BOOST_CHECK(holds_alternative<Nil>(it->variant()));

        ++it;
        BOOST_REQUIRE(it != end);
        BOOST_CHECK(holds_alternative<Boolean>(it->variant()));
        BOOST_CHECK_EQUAL(get<Boolean>(it->variant()).value(), true);

        ++it;
        BOOST_REQUIRE(it != end);
        BOOST_CHECK(holds_alternative<Int>(it->variant()));
        BOOST_CHECK_EQUAL(get<Int>(it->variant()).value(), 42);

        ++it;
        BOOST_CHECK(it == end);
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(xmlrpc_types_Struct)

BOOST_AUTO_TEST_CASE(Base) {
    Struct s;
    const bool check = std::is_same<decltype(s)::value_type, std::map<std::string, Value>>::value;
    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(Constructor) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
    }
    {
        Array a;
        BOOST_CHECK_EQUAL(a.size(), 0);
        a.push_back(Nil{});
        a.push_back(Nil{});
        BOOST_CHECK_EQUAL(a.size(), 2);
        Array a2{a};
        BOOST_CHECK_EQUAL(a2.size(), 2);
    }
}

BOOST_AUTO_TEST_CASE(UnknownKey) {
    {
        const Struct s;
        BOOST_CHECK_THROW(auto &v = s.get_value("0"), std::invalid_argument);
        BOOST_CHECK_THROW(auto &v = s.get<Nil>("0"), std::invalid_argument);
    }
    {
        Struct s;
        BOOST_CHECK_THROW(auto &v = s.get_value("0"), std::invalid_argument);
        BOOST_CHECK_THROW(auto &v = s.get<Nil>("0"), std::invalid_argument);
        std::string key = "0";
        BOOST_CHECK_THROW(auto &v = s.get_value(key), std::invalid_argument);
        BOOST_CHECK_THROW(auto &v = s.get<Nil>("0"), std::invalid_argument);
        s.insert("0", Nil{});
        BOOST_CHECK_NO_THROW(auto &v = s.get_value("0"));
        BOOST_CHECK_NO_THROW(auto &v = s.get<Nil>("0"));
        {
            const Struct &s2 = s;
            BOOST_CHECK_NO_THROW(auto &v = s.get_value("0"));
            BOOST_CHECK_NO_THROW(auto &v = s.get<Nil>("0"));
        }
    }
}

BOOST_AUTO_TEST_CASE(InsertNil) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("nil0", Nil{});
        BOOST_CHECK_EQUAL(s.size(), 1);

        const auto &g = s.get_value("nil0");
        BOOST_REQUIRE(holds_alternative<Nil>(g));
        auto &n1 = get<Nil>(g);
    }
    {
        Struct s;
        Nil nil;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("nil0", nil);
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("nil0");
        BOOST_REQUIRE(holds_alternative<Nil>(g));
        auto &n1 = get<Nil>(g);
        auto &v = s.get<Nil>("nil0");
    }
    {
        Struct s;
        Nil nil;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("nil0", nil);
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        s.insert("nil1", nil);
        BOOST_REQUIRE_EQUAL(s.size(), 2);
        const auto &g = s.get_value("nil0");
        BOOST_REQUIRE(holds_alternative<Nil>(g));
        auto &n1 = get<Nil>(g);
        BOOST_REQUIRE(holds_alternative<Nil>(s.get_value("nil0")));
        BOOST_REQUIRE(holds_alternative<Nil>(s.get_value("nil1")));
    }
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("nil0", Nil{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Nil>(s.get_value("nil0")));
        s.insert("nil0", Nil{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Nil>(s.get_value("nil0")));
    }
}

BOOST_AUTO_TEST_CASE(InsertInt) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("int0", Int{42});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("int0");
        BOOST_REQUIRE(holds_alternative<Int>(g));
        BOOST_CHECK_EQUAL(get<Int>(g).value(), 42);
    }
    {
        Struct s;
        Int i{42};
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("int0", i);
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("int0");
        BOOST_REQUIRE(holds_alternative<Int>(g));
        const auto &i2 = get<Int>(g);
        BOOST_CHECK_EQUAL(i2.value(), 42);
        i.set_value(82);
        BOOST_CHECK_EQUAL(i2.value(), 42);
        auto &v = s.get<Int>("int0");
        BOOST_CHECK_EQUAL(v.value(), 42);
    }
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("v", Nil{});
        BOOST_CHECK_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Nil>(s.get_value("v")));
        s.insert("v", Int{});
        BOOST_CHECK_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Int>(s.get_value("v")));
    }
}

BOOST_AUTO_TEST_CASE(InsertDouble) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("d0", Double{1.5});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("d0");
        BOOST_REQUIRE(holds_alternative<Double>(g));
        BOOST_CHECK_EQUAL(get<Double>(g).value(), 1.5);
    }
    {
        Struct s;
        Double i{4.25};
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("int0", i);
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("int0");
        BOOST_REQUIRE(holds_alternative<Double>(g));
        const auto &i2 = get<Double>(g);
        BOOST_CHECK_EQUAL(i2.value(), 4.25);
        i.set_value(-9.5);
        BOOST_CHECK_EQUAL(i2.value(), 4.25);
        auto &v = s.get<Double>("int0");
        BOOST_CHECK_EQUAL(v.value(), 4.25);
    }
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("v", Double{4.12});
        BOOST_CHECK_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Double>(s.get_value("v")));
        s.insert("v", Nil{});
        BOOST_CHECK_EQUAL(s.size(), 1);
        BOOST_CHECK(holds_alternative<Nil>(s.get_value("v")));
    }
}

BOOST_AUTO_TEST_CASE(InsertString) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("str0", String{"hello string"});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("str0");
        BOOST_REQUIRE(holds_alternative<String>(g));
        BOOST_CHECK_EQUAL(get<String>(g).value(), "hello string");
        auto &v = s.get<String>("str0");
        BOOST_CHECK_EQUAL(v.value(), "hello string");
    }
    {
        Struct s;
        String str{"long string"};
        BOOST_CHECK_EQUAL(s.size(), 0);
        s.insert("str0", str);
        BOOST_CHECK_EQUAL(str.value(), "long string");
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("str0");
        BOOST_REQUIRE(holds_alternative<String>(g));
        const auto &str2 = get<String>(g);
        BOOST_CHECK_EQUAL(str2.value(), "long string");
        str.value() += " even longer";
        BOOST_CHECK_EQUAL(str.value(), "long string even longer");
        BOOST_CHECK_EQUAL(str2.value(), "long string");
        auto &v = s.get<String>("str0");
        BOOST_CHECK_EQUAL(v.value(), "long string");
    }
}

BOOST_AUTO_TEST_CASE(InsertArray) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        auto &a_child = s.insert("a0", Array{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("a0");
        BOOST_REQUIRE(holds_alternative<Array>(g));
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        a_child.push_back(Nil{});
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
    }
    {
        Struct s;
        Struct s2;
        BOOST_CHECK_EQUAL(s.size(), 0);
        auto &a_child = s.insert("a0", Array{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("a0");
        BOOST_REQUIRE(holds_alternative<Array>(g));
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);
        BOOST_CHECK_EQUAL(s2.size(), 0);

        s2.insert("n0", Nil{});
        BOOST_CHECK_EQUAL(s2.size(), 1);
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 0);
        BOOST_CHECK_EQUAL(a_child.size(), 0);

        a_child.push_back(Nil{});
        BOOST_CHECK_EQUAL(s2.size(), 1);
        BOOST_CHECK_EQUAL(get<Array>(g).size(), 1);
        BOOST_CHECK_EQUAL(a_child.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(InsertStruct) {
    {
        Struct s;
        BOOST_CHECK_EQUAL(s.size(), 0);
        auto &s_child = s.insert("s0", Struct{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("s0");
        BOOST_REQUIRE(holds_alternative<Struct>(g));
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(s_child.size(), 0);
        s_child.insert("s0", Nil{});
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 1);
        BOOST_CHECK_EQUAL(s_child.size(), 1);
    }
    {
        Struct s;
        Struct s2;
        BOOST_CHECK_EQUAL(s.size(), 0);
        auto &s_child = s.insert("s0", Struct{});
        BOOST_REQUIRE_EQUAL(s.size(), 1);
        const auto &g = s.get_value("s0");
        BOOST_REQUIRE(holds_alternative<Struct>(g));
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(s_child.size(), 0);
        BOOST_CHECK_EQUAL(s2.size(), 0);

        s2.insert("n0", Nil{});
        BOOST_CHECK_EQUAL(s2.size(), 1);
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 0);
        BOOST_CHECK_EQUAL(s_child.size(), 0);

        s_child.insert("s0", Nil{});
        BOOST_CHECK_EQUAL(s2.size(), 1);
        BOOST_CHECK_EQUAL(get<Struct>(g).size(), 1);
        BOOST_CHECK_EQUAL(s_child.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(ForLoop) {
    {
        Struct s;
        static constexpr auto NB = 5;
        for (size_t i = 0; i < NB; ++i) {
            int v = 1 << i;
            s.insert(std::to_string(i), Int{v});
        }
        int iteration = 0;
        for (const auto &[key, value] : s) {
            bool b = std::is_same<std::decay<decltype(key)>::type, std::string>::value;
            BOOST_CHECK(b);
            bool b2 = std::is_same<std::decay<decltype(value)>::type, Value>::value;
            BOOST_CHECK(b2);

            const int ref = 1 << iteration;
            BOOST_REQUIRE(holds_alternative<Int>(value));
            int i = get<Int>(value).value();
            BOOST_CHECK_EQUAL(i, s.get<Int>(key).value());
            BOOST_CHECK_EQUAL(i, ref);
            ++iteration;
        }
        BOOST_CHECK_EQUAL(iteration, NB);
    }
}

BOOST_AUTO_TEST_CASE(Iterator) {
    {
        Struct a;
        a.insert("arg0", Nil{});
        a.insert("arg1", Boolean{true});
        a.insert("arg2", Int{42});

        auto it = a.begin();
        auto end = a.end();

        BOOST_REQUIRE(it != end);
        BOOST_CHECK_EQUAL(it->first, "arg0");
        BOOST_CHECK(holds_alternative<Nil>(it->second));

        ++it;
        BOOST_REQUIRE(it != end);
        BOOST_CHECK_EQUAL(it->first, "arg1");
        BOOST_CHECK(holds_alternative<Boolean>(it->second));
        BOOST_CHECK_EQUAL(get<Boolean>(it->second).value(), true);

        ++it;
        BOOST_REQUIRE(it != end);
        BOOST_CHECK_EQUAL(it->first, "arg2");
        BOOST_CHECK(holds_alternative<Int>(it->second));
        BOOST_CHECK_EQUAL(get<Int>(it->second).value(), 42);

        ++it;
        BOOST_CHECK(it == end);
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ValueTests)

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Nil) {
    Value v;
    BOOST_CHECK(holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Boolean) {
    Value v{false};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_REQUIRE(holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));

    BOOST_CHECK_EQUAL(get<Boolean>(v).value(), false);
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Int) {
    Value v{Int{434}};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_REQUIRE(holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));

    BOOST_CHECK_EQUAL(get<Int>(v).value(), 434);
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Double) {
    Value v{Double{4.5}};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_REQUIRE(holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));

    BOOST_CHECK_EQUAL(get<Double>(v).value(), 4.5);
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_String) {
    Value v{String{"whatever"}};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_REQUIRE(holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));

    BOOST_CHECK_EQUAL(get<String>(v).value(), "whatever");
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Array) {
    Value v{Array{}};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_REQUIRE(holds_alternative<Array>(v));
    BOOST_CHECK(!holds_alternative<Struct>(v));
}

BOOST_AUTO_TEST_CASE(HoldsAlternative_Get_Struct) {
    Value v{Struct{}};
    BOOST_CHECK(!holds_alternative<Nil>(v));
    BOOST_CHECK(!holds_alternative<Boolean>(v));
    BOOST_CHECK(!holds_alternative<Int>(v));
    BOOST_CHECK(!holds_alternative<Double>(v));
    BOOST_CHECK(!holds_alternative<String>(v));
    BOOST_CHECK(!holds_alternative<Array>(v));
    BOOST_REQUIRE(holds_alternative<Struct>(v));
}

BOOST_AUTO_TEST_SUITE_END()

