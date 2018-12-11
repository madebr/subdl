#ifndef XMLRPC_RAPIDXML_HPP
#define XMLRPC_RAPIDXML_HPP

#include <rapidxml.h>

#include "types.hpp"

#include <type_traits>

#include <iostream> // FIXME

namespace xmlrpc {
namespace rapidxml {

struct ToXmlConverter {
    static ::rapidxml::xml_node<> *append_xml(::rapidxml::xml_node<> *parent, const Nil &);

    template <typename T, typename = std::enable_if<std::is_base_of<ValueType<typename T::value_type>, typename std::decay<T>::type>::value>>
    static ::rapidxml::xml_node<> *append_xml(::rapidxml::xml_node<> *parent, const T &node) {
        auto &document = *parent->document();

        auto stl_str = Types::Info<typename T::value_type>::to_string(node.value());
        auto str = document.allocate_string(stl_str.c_str());
        auto scalar_node = document.allocate_node(::rapidxml::node_element, Types::Info<typename T::value_type>::name.data(), str);
        parent->append_node(scalar_node);

        return scalar_node;
    }

    static ::rapidxml::xml_node<> *append_xml(::rapidxml::xml_node<> *parent, const Array &node);

    static ::rapidxml::xml_node<> *append_xml(::rapidxml::xml_node<> *parent, const Struct &node);

    static ::rapidxml::xml_node<> *append_value_xml(::rapidxml::xml_node<> *parent, const Value &value);
};

}
}

#endif // XMLRPC_RAPIDXML_HPP
