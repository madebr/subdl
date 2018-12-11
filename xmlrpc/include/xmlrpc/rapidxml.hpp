#ifndef XMLRPC_RAPIDXML_HPP
#define XMLRPC_RAPIDXML_HPP

#include <xmlrpc/xmlrpc.hpp>

#include <rapidxml.h>

#include "types.hpp"

#include <type_traits>

#include <iostream> // FIXME

namespace xmlrpc {
namespace rapidxml {

template <typename Ch=char>
struct ValueToXmlConverter {
    static ::rapidxml::xml_node<Ch> *append_xml(::rapidxml::xml_node<Ch> *parent, const Nil &) {
        auto &document = *parent->document();

        auto nil_node = document.allocate_node(::rapidxml::node_element, Types::Info<nullptr_t>::name.data());
        parent->append_node(nil_node);

        return nil_node;
    }

    template <typename T, typename = std::enable_if<std::is_base_of<ValueType<typename T::value_type>, typename std::decay<T>::type>::value>>
    static ::rapidxml::xml_node<Ch> *append_xml(::rapidxml::xml_node<Ch> *parent, const T &node) {
        auto &document = *parent->document();

        auto stl_str = Types::Info<typename T::value_type>::to_string(node.value());
        auto str = document.allocate_string(stl_str.c_str(), 1+stl_str.length());
        auto scalar_node = document.allocate_node(::rapidxml::node_element, Types::Info<typename T::value_type>::name.data(), str);
        parent->append_node(scalar_node);

        return scalar_node;
    }

    static ::rapidxml::xml_node<Ch> *append_xml(::rapidxml::xml_node<Ch> *parent, const Array &node) {
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
        return array_node;}

    static ::rapidxml::xml_node<Ch> *append_xml(::rapidxml::xml_node<Ch> *parent, const Struct &node) {
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

    static ::rapidxml::xml_node<Ch> *append_value_xml(::rapidxml::xml_node<Ch> *parent, const Value &value) {
        return std::visit([parent](const auto &arg) {
            return append_xml(parent, arg);
        }, value.variant());
    }
};

template <typename Ch=char>
struct XmlToValueConverter {
    static Value extract_value(::rapidxml::xml_node<Ch> *node) {
        if (node == nullptr) {
            return Nil{};
        }
        std::string_view node_name{node->name(), node->name_size()};
        if (node_name == Types::Info<nullptr_t>::name) {
            return Nil{};
        } else if (node_name == Types::Info<bool>::name) {
            std::string intstring{node->value(), node->value_size()};
            return Boolean{std::stoi(intstring) != 0 ? true : false};
        } else if (node_name == Types::Info<int>::name) {
            std::string intstring{node->value(), node->value_size()};
            return Int{std::stoi(intstring)};
        } else if (node_name == Types::Info<double>::name) {
            std::string doublestring{node->value(), node->value_size()};
            return Double{std::stod(doublestring)};
        }  else if (node_name == Types::Info<std::string>::name) {
            return String{std::string(node->value(), node->value_size())};
        } else if (node_name == Types::Info<std::vector<Value>>::name) {
            return extract_array(node);
        } else if (node_name == Types::Info<std::map<std::string, Value>>::name) {
            return extract_struct(node);
        } else {
            // FIXME: add logging
            return Nil{};
        }
    }

    static Struct extract_struct(::rapidxml::xml_node<Ch> *node) {
        Struct s;
        auto memberNode = node->first_node("member");
        while (memberNode != nullptr) {
            auto nameNode = memberNode->first_node("name");
            if (nameNode == nullptr) {
                continue;
            }
            std::string key{nameNode->value(), nameNode->value_size()};
            auto valueNode = memberNode->first_node("value");
            Value value;
            if (valueNode != nullptr) {
                value = std::move(extract_value(valueNode->first_node()));
            }
            s.emplace(std::move(key), std::move(value));
            memberNode = memberNode->next_sibling("member");
        }
        return s;
    }

    static Array extract_array(::rapidxml::xml_node<Ch> *node) {
        Array a;
        auto dataNode = node->first_node("data");
        if (dataNode == nullptr) {
            return a;
        }
        auto valueNode = dataNode->first_node("value");
        while (valueNode != nullptr) {
            auto value = extract_value(valueNode->first_node());
            a.emplace_back(std::move(value));

            valueNode = valueNode->next_sibling("value");
        }
        return a;
    }
};


template <typename Ch=char>
struct MethodToXmlConverter {
    static ::rapidxml::xml_node<Ch> *append_xml(::rapidxml::xml_node<Ch> *parent, const CallMethod &args) {
        auto &document = *parent->document();

        auto methodCallNode = document.allocate_node(::rapidxml::node_element, "methodCall");
        parent->append_node(methodCallNode);

        auto methodNameString = document.allocate_string(args.name().c_str(), 1+args.name().length());
        auto methodNameNode = document.allocate_node(::rapidxml::node_element, "methodName", methodNameString);
        methodCallNode->append_node(methodNameNode);

        auto paramsNode = document.allocate_node(::rapidxml::node_element, "params");
        methodCallNode->append_node(paramsNode);

        for (const auto &v : args.data()){
            auto paramNode = document.allocate_node(::rapidxml::node_element, "value");
            paramsNode->append_node(paramNode);

            ValueToXmlConverter<Ch>::append_value_xml(paramNode, v);
        }

        return methodCallNode;
    }
};

}
}

#endif // XMLRPC_RAPIDXML_HPP
