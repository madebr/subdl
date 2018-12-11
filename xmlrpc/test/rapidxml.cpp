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
    auto node = ToXmlConverter::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<nil/>");
}

BOOST_AUTO_TEST_CASE(BooleanTests) {
    {
        Boolean v{false};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<boolean>0</boolean>");
    }
    {
        Boolean v{true};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<boolean>1</boolean>");
    }
}

BOOST_AUTO_TEST_CASE(IntTests) {
    Int v{42};
    ::rapidxml::xml_document<> document;
    auto node = ToXmlConverter::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<int>42</int>");
}

BOOST_AUTO_TEST_CASE(DoubleTests) {
    xmlrpc::Double v{4.210987};
    ::rapidxml::xml_document<> document;
    auto node = ToXmlConverter::append_xml(&document, v);

    std::ostringstream oss;
    ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
    BOOST_CHECK_EQUAL(oss.str(), "<double>4.210987</double>");
}

BOOST_AUTO_TEST_CASE(StringTests) {
    {
        String v{};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string/>");
    }
    {
        String v{"what a nice string"};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>what a nice string</string>");
    }
    {
        String v{"</string>"};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>&lt;/string&gt;</string>");
    }
    {
        String v{"hallo mr \"nobody\""};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>hallo mr &quot;nobody&quot;</string>");
    }
    {
        String v{"it's a nice day"};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<string>it&apos;s a nice day</string>");
    }
}

BOOST_AUTO_TEST_CASE(ArrayTests) {
    {
        Array v{};
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<array><data/></array>");
    }
    {
        Array v{};
        v.push_back(Nil{});
        ::rapidxml::xml_document<> document;
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

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
        auto node = ToXmlConverter::append_xml(&document, v);

        std::ostringstream oss;
        ::rapidxml::print<char>(oss, *node, ::rapidxml::print_no_indenting);
        BOOST_CHECK_EQUAL(oss.str(), "<struct><member>"
                                     "<name>arg0</name><value><nil/></value></member><member>"
                                     "<name>arg1</name><value><int>42</int></value></member><member>"
                                     "<name>arg2</name><value><boolean>1</boolean></value>"
                                     "</member></struct>");
    }
}

//BOOST_AUTO_TEST_CASE(main) {
////    Int smain{3};
//    xmlrpc::Struct smain{};
//    smain.insert("par0", xmlrpc::Nil{});
//    smain.insert("par1", xmlrpc::Int{3});
//    smain.insert("par2", xmlrpc::Boolean{true});
//    smain.insert("par3", xmlrpc::Boolean{false});
//    smain.insert("par4", xmlrpc::Double{4.456});
//    smain.insert("par5", xmlrpc::String{"abc"});
//    auto &par5struct = smain.insert("par6", xmlrpc::Struct{});
//    auto &array = par5struct.insert("par7", xmlrpc::Array{});
//    array.push_back(xmlrpc::Int{5});
//    array.push_back(xmlrpc::Int{6});
//    array.push_back(xmlrpc::Int{7});
//    array.push_back(xmlrpc::Int{8});
//    array.push_back(xmlrpc::Int{8});

//    ::rapidxml::xml_document document{};
//    ToXmlConverter::append_xml(smain, &document);
//    std::cout << document;
//}

BOOST_AUTO_TEST_SUITE_END()
