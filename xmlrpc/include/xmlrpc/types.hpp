#ifndef XMLRPC_TYPES_HPP
#define XMLRPC_TYPES_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace xmlrpc {

class Value;

namespace Types {
template <typename T>
struct Info {
};
template <>
struct Info<nullptr_t> {
    static constexpr std::string_view name = "nil";
};
template <>
struct Info<bool> {
    static constexpr std::string_view name = "boolean";
    static std::string to_string(bool b) {
        return b ? "1" : "0";
    }
};
template <>
struct Info<int> {
    static constexpr std::string_view name = "int";
    static std::string to_string(int i) {
        return std::to_string(i);
    }
};
template <>
struct Info<double> {
    static constexpr std::string_view name = "double";
    static std::string to_string(double d) {
        return std::to_string(d);
    }
};
template <>
struct Info<std::string> {
    static constexpr std::string_view name = "string";
    static const std::string to_string(const std::string &s) {
        // copy to avoid creating a reference between model and view
        return std::string(s);
    }
};
template <>
struct Info<std::vector<Value>> {
    static constexpr std::string_view name = "array";
};
template <>
struct Info<std::map<std::string, Value>> {
    static constexpr std::string_view name = "struct";
};
}

class XmlRpcType {
};

class Nil : public XmlRpcType {
public:
    Nil() = default;
    Nil(nullptr_t) : Nil() {}
    using value_type = nullptr_t;
};

template <typename T>
class ValueType : public XmlRpcType {
public:
    using value_type = T;
    ValueType(const T &t)
        : data_(t) {
    }
    ValueType(T &&t)
        : data_(std::move(t)) {
    }
    ValueType()
        : data_{} {
    }
protected:
    T data_;
};

template <typename T>
class ScalarType : public ValueType<T> {
public:
    using value_type = T;  // typename ValueType<T>::value_type;
    ScalarType() = default;
    ScalarType(const T &t) : ValueType<T>(t) {}
    ScalarType(T &&t) : ValueType<T>(std::move(t)) {}
};

class Boolean : public ScalarType<bool> {
public:
    [[nodiscard]]value_type value() const {
        return data_;
    }
    void set_value(value_type val) {
        data_ = val;
    }
};

class Int : public ScalarType<int> {
public:
    [[nodiscard]] value_type value() const {
        return data_;
    }
    void set_value(value_type val) {
        data_ = val;
    }
};

class Double : public ScalarType<double> {
public:
    [[nodiscard]] value_type value() const {
        return data_;
    }
    void set_value(value_type val) {
        data_ = val;
    }
};

class String : public ScalarType<std::string> {
public:
    String() : ScalarType<value_type>() {}
    String(const std::string &s) : ScalarType<value_type>(s) {}
    String(std::string &&s) : ScalarType<value_type>(std::move(s)) {}
    String(const char *s) : ScalarType<value_type>(s) {}
public:
    [[nodiscard]] const value_type &value() const {
        return data_;
    }
    [[nodiscard]] value_type &value() {
        return data_;
    }
    void set_value(const value_type &val) {
        data_ = val;
    }
    void set_value(value_type &&val) {
        data_ = std::move(val);
    }
};

class Array : public XmlRpcType {
public:
    using value_type = std::vector<Value>;
    Array() {
    }
    // FIXME + TEST: need copy + move constructor?
    template <typename T>
    T &push_back(T &&v);
    template <typename... Targs>
    Value &emplace_back(Targs &&...args);

    [[nodiscard]] const Value &operator [](size_t i) const {
        return values_[i];
    }
    [[nodiscard]] Value &operator [](size_t i) {
        return values_[i];
    }
    template <typename T>
    [[nodiscard]] T &get(size_t i);
    template <typename T>
    [[nodiscard]] const T &get(size_t i) const;
    [[nodiscard]] std::size_t size() const {
        return values_.size();
    }

    [[nodiscard]] value_type::const_iterator begin() const {
        return values_.begin();
    }
    [[nodiscard]] value_type::const_iterator end() const {
        return values_.end();
    }
private:
    value_type values_;
};

class Struct : public XmlRpcType{
public:
    using key_type = std::string;
    using value_type = std::map<key_type, Value>;
    Struct() {
    }
    // FIXME + TEST: need copy + move constructor?

    template <typename TKey, typename T>
    T &insert(TKey &&key, T &&value);
    template <typename TKey, typename... TArgs>
    Value &emplace(TKey &&key, TArgs&& ... args);

    template <typename TKey>
    [[nodiscard]] const Value &get_value(TKey &&key) const;
    template <typename TKey>
    [[nodiscard]] Value &get_value(TKey &&key);

    template <typename T, typename TKey>
    [[nodiscard]] const T &get(TKey &&key) const;
    template <typename T, typename TKey>
    [[nodiscard]] T &get(TKey &&key);


    std::size_t size() const {
        return members_.size();
    }

    [[nodiscard]] value_type::const_iterator begin() const {
        return members_.cbegin();
    }
    [[nodiscard]] value_type::const_iterator end() const {
        return members_.cend();
    }
private:
    value_type members_;
};

class Value {
public:
    Value() : variant_(Nil{}) {}
    Value(nullptr_t) : variant_(Nil{}) {}
    template <typename T, size_t I=0, typename = typename std::enable_if<std::is_base_of<XmlRpcType, typename std::decay<T>::type>::value, T>::type>
    Value(T &&arg) : variant_(std::forward<T>(arg)) {}
//    template <typename T, typename = typename std::enable_if<std::is_base_of<XmlRpcType, typename std::decay<T>::type>::value, T>::type>
//    Value(auto arg) : variant_(std::forward(arg)) {}
    Value(bool b) : variant_(Boolean{b}) {}
    Value(int i) : variant_(Int{i}) {}
    Value(double d) : variant_(Double{d}) {}
    template <typename TString, typename = typename std::enable_if<!std::is_same<String, typename std::decay<TString>::type>::value && std::is_constructible<String, TString>::value, TString>::type>
    Value(TString &&s) : variant_(String{std::forward<TString>(s)}) {}
//    template <typename TString, typename = typename std::enable_if<std::is_constructible<String, TString>::value, TString>::type>
//    Value(auto s) : variant_(String{std::forward(s)}) {}

    using value_type = std::variant<Nil, Boolean, Int, Double, String, Array, Struct>;
    const value_type &variant() const;
private:
    value_type &variant();
    value_type variant_;
    template <typename T>
    bool holds_alternative(const xmlrpc::Value &v);
    template <typename T>
    friend const T &get(const xmlrpc::Value &v);
    template <typename T>
    friend T &get(xmlrpc::Value &v);
};

template <typename T>
[[nodiscard]] bool holds_alternative(const xmlrpc::Value &v) {
    return std::holds_alternative<T>(v.variant());
}

template <typename T>
[[nodiscard]] const T &get(const xmlrpc::Value &v) {
    return std::get<T>(v.variant());
}

template <typename T>
[[nodiscard]] T &get(xmlrpc::Value &v) {
    return std::get<T>(v.variant());
}

template <typename T>
T &Array::get(size_t i) {
    return xmlrpc::get<T>(values_[i]);
}

template <typename T>
T &Array::push_back(T &&v) {
    values_.push_back(Value{std::forward<T>(v)});
    return xmlrpc::get<typename std::decay<T>::type>(values_.back());
}

template <typename... Targs>
Value &Array::emplace_back(Targs &&...args) {
    return values_.emplace_back(std::forward<Targs>(args)...);
}

template <typename TKey, typename T>
T &Struct::insert(TKey &&key, T &&value) {
    auto it_bool = members_.insert_or_assign(std::forward<TKey>(key), Value{std::forward<T>(value)});
    return xmlrpc::get<typename std::decay<T>::type>(it_bool.first->second);
}

template <typename TKey, typename... TArgs>
Value &Struct::emplace(TKey &&key, TArgs&& ... args) {
    auto it_bool = members_.emplace(std::forward<TKey>(key), std::forward<TArgs>(args)...);
    return it_bool.first->second;
}

template <typename TKey>
const Value &Struct::get_value(TKey &&key) const {
    const auto it = members_.find(std::forward<TKey>(key));
    if (it == members_.end()) {
        throw std::invalid_argument{std::forward<TKey>(key)};
    }
    return it->second;
}

template <typename TKey>
Value &Struct::get_value(TKey &&key) {
    const auto it = members_.find(std::forward<TKey>(key));
    if (it == members_.end()) {
        throw std::invalid_argument{std::forward<TKey>(key)};
    }
    return it->second;
}

template <typename T, typename TKey>
const T &Struct::get(TKey &&key) const {
    return xmlrpc::get<T>(get_value(std::forward<TKey>(key)));
}

template <typename T, typename TKey>
T &Struct::get(TKey &&key) {
    return xmlrpc::get<T>(get_value(std::forward<TKey>(key)));
}

}
#endif // XMLRPC_TYPES_HPP
