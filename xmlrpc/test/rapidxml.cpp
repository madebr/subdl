#include <boost/test/unit_test.hpp>

#include <xmlrpc/rapidxml.hpp>

#include <iostream>
#include <sstream>
#include <rapidxml_print.h>

using namespace xmlrpc;
using namespace xmlrpc::rapidxml;

BOOST_AUTO_TEST_SUITE(rapidxml_toxml)

BOOST_AUTO_TEST_CASE(NilTests) {
    Nil v{};
    ::rapidxml::xml_document<> document;
    auto node = ValueToXmlConverter<>::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<nil/>");
}

BOOST_AUTO_TEST_CASE(BooleanTests) {
    {
        Boolean v{false};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<boolean>0</boolean>");
    }
    {
        Boolean v{true};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<boolean>1</boolean>");
    }
}

BOOST_AUTO_TEST_CASE(IntTests) {
    Int v{42};
    ::rapidxml::xml_document<> document;
    auto node = ValueToXmlConverter<>::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<int>42</int>");
}

BOOST_AUTO_TEST_CASE(DoubleTests) {
    xmlrpc::Double v{4.210987};
    ::rapidxml::xml_document<> document;
    auto node = ValueToXmlConverter<>::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<double>4.210987</double>");
}

BOOST_AUTO_TEST_CASE(StringTests) {
    {
        String v{};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string/>");
    }
    {
        String v{"what a nice string"};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>what a nice string</string>");
    }
    {
        String v{"</string>"};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>&lt;/string&gt;</string>");
    }
    {
        String v{"hallo mr \"nobody\""};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>hallo mr &quot;nobody&quot;</string>");
    }
    {
        String v{"it's a nice day"};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>it&apos;s a nice day</string>");
    }
}

BOOST_AUTO_TEST_CASE(ArrayTests) {
    {
        Array v{};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data/></array>");
    }
    {
        Array v{};
        v.push_back(Nil{});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><nil/></value>"
                                     "</data></array>");

    }
    {
        Array v{};
        v.push_back(Double{3.141592});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><double>3.141592</double></value>"
                                     "</data></array>");

    }
    {
        Array v{};
        v.push_back(String{"string in array"});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><string>string in array</string></value>"
                                     "</data></array>");

    }
    {
        Array v{};
        v.push_back(Array{});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><array><data/></array></value>"
                                     "</data></array>");

    }
    {
        Array v{};
        v.push_back(Struct{});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><struct/></value>"
                                     "</data></array>");

    }
    {
        Array v{};
        v.push_back(Nil{});
        v.push_back(Int{5});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data>"
                                     "<value><nil/></value><value><int>5</int></value>"
                                     "</data></array>");
    }
}

BOOST_AUTO_TEST_CASE(StructTests) {
    {
        Struct v{};
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<struct/>");
    }
    {
        Struct v{};
        v.insert("arg0", Nil{});
        v.insert("arg1", Int{42});
        v.insert("arg2", Boolean{true});
        ::rapidxml::xml_document<> document;
        auto node = ValueToXmlConverter<>::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<struct><member>"
                                     "<name>arg0</name><value><nil/></value></member><member>"
                                     "<name>arg1</name><value><int>42</int></value></member><member>"
                                     "<name>arg2</name><value><boolean>1</boolean></value>"
                                     "</member></struct>");
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestXmlRpc)

BOOST_AUTO_TEST_CASE(NoArgs) {
    ::rapidxml::xml_node<> *node;
    {
        CallMethod method("noargsfunction");
        method.call();
        ::rapidxml::xml_document<> document;
        node = MethodToXmlConverter<>::append_xml(&document, method);
    }

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<methodCall>"
                                 "<methodName>noargsfunction</methodName>"
                                 "<params/>"
                                 "</methodCall>");
}

BOOST_AUTO_TEST_CASE(BoolArgs) {
    ::rapidxml::xml_node<> *node;
    {
        CallMethod method("boolsmethod");
        method.call(true, false, true);
        ::rapidxml::xml_document<> document;
        node = MethodToXmlConverter<>::append_xml(&document, method);
    }

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<methodCall>"
                                 "<methodName>boolsmethod</methodName>"
                                 "<params>"
                                 "<value><boolean>1</boolean></value>"
                                 "<value><boolean>0</boolean></value>"
                                 "<value><boolean>1</boolean></value>"
                                 "</params>"
                                 "</methodCall>");
}

BOOST_AUTO_TEST_CASE(IntArgs) {
    ::rapidxml::xml_node<> *node;
    {
        CallMethod method("intsmethod");
        method.call(95, -4, 256);
        ::rapidxml::xml_document<> document;
        node = MethodToXmlConverter<>::append_xml(&document, method);
    }

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<methodCall>"
                                 "<methodName>intsmethod</methodName>"
                                 "<params>"
                                 "<value><int>95</int></value>"
                                 "<value><int>-4</int></value>"
                                 "<value><int>256</int></value>"
                                 "</params>"
                                 "</methodCall>");
}

BOOST_AUTO_TEST_CASE(StringArgs) {
    ::rapidxml::xml_node<> *node;
    {
        CallMethod method("stringsmethod");
        method.call("hallo", "chicken", std::string{"dinner<>"});
        ::rapidxml::xml_document<> document;
        node = MethodToXmlConverter<>::append_xml(&document, method);
    }

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<methodCall>"
                                 "<methodName>stringsmethod</methodName>"
                                 "<params>"
                                 "<value><string>hallo</string></value>"
                                 "<value><string>chicken</string></value>"
                                 "<value><string>dinner&lt;&gt;</string></value>"
                                 "</params>"
                                 "</methodCall>");
}

BOOST_AUTO_TEST_CASE(ArgsMix) {
    ::rapidxml::xml_node<> *node;
    {
        CallMethod method("argsmixmethod");
        method.call(nullptr, true, 20, 4.6, "text");
        ::rapidxml::xml_document<> document;
        node = MethodToXmlConverter<>::append_xml(&document, method);
    }

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<methodCall>"
                                 "<methodName>argsmixmethod</methodName>"
                                 "<params>"
                                 "<value><nil/></value>"
                                 "<value><boolean>1</boolean></value>"
                                 "<value><int>20</int></value>"
                                 "<value><double>4.600000</double></value>"
                                 "<value><string>text</string></value>"
                                 "</params>"
                                 "</methodCall>");
}

BOOST_AUTO_TEST_SUITE_END()
