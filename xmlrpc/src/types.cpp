#include <xmlrpc/types.hpp>

namespace xmlrpc {

const Value::value_type &Value::variant() const {
    return variant_;
}
Value::value_type &Value::variant() {
    return variant_;
}

}
