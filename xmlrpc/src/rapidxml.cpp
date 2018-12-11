#include <xmlrpc/rapidxml.hpp>

namespace xmlrpc {
namespace rapidxml {

::rapidxml::xml_node<> *ToXmlConverter::append_xml(::rapidxml::xml_node<> *parent, const Nil &) {
    auto &document = *parent->document();

    auto nil_node = document.allocate_node(::rapidxml::node_element, Types::Info<nullptr_t>::name.data());
    parent->append_node(nil_node);

    return nil_node;
}

::rapidxml::xml_node<> *ToXmlConverter::append_xml(::rapidxml::xml_node<> *parent, const Array &node) {
    auto &document = *parent->document();
    auto array_node = document.allocate_node(::rapidxml::node_element, Types::Info<Array::value_type>::name.data());
    parent->append_node(array_node);
    auto data_node = document.allocate_node(::rapidxml::node_element, "data");
    array_node->append_node(data_node);
    for (const auto& item : node) {
        auto value_node = document.allocate_node(::rapidxml::node_element, "value");
        data_node->append_node(value_node);

        append_value_xml(value_node, item);
    }
    return array_node;
}

::rapidxml::xml_node<> *ToXmlConverter::append_xml(::rapidxml::xml_node<> *parent, const Struct &node) {
    auto &document = *parent->document();
    auto struct_node = document.allocate_node(::rapidxml::node_element, Types::Info<Struct::value_type>::name.data());
    parent->append_node(struct_node);
    for (const auto& [key, value] : node) {
        auto member_node = document.allocate_node(::rapidxml::node_element, "member");
        struct_node->append_node(member_node);

        auto name_node = document.allocate_node(::rapidxml::node_element, "name", key.c_str(), 0, key.length());
        member_node->append_node(name_node);

        auto value_node = document.allocate_node(::rapidxml::node_element, "value");
        member_node->append_node(value_node);

        append_value_xml(value_node, value);
    }
    return struct_node;
}

::rapidxml::xml_node<> *ToXmlConverter::append_value_xml(::rapidxml::xml_node<> *parent, const Value &value) {
    return std::visit([parent](const auto &arg) {
        return append_xml(parent, arg);
    }, value.variant());
}

}
}
