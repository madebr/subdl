#ifndef XMLRPC_XMLRPC_HPP
#define XMLRPC_XMLRPC_HPP

#include "types.hpp"

#include <string>
#include <vector>

namespace xmlrpc {

class CallMethod {
    std::string name_;
    std::vector<Value> data_;
public:
    template <typename T>
    CallMethod(T &&name) : name_(std::forward<T>(name)) {
    }

    template <typename... TArgs>
    void call(TArgs&&...args) {
        MethodArgsData argsObj;
        argsObj.call<sizeof...(args)>(std::forward<TArgs>(args)...);
        data_ = std::move(argsObj.values);
    }

    const std::string &name() const noexcept {
        return name_;
    }

    const std::vector<Value> &data() const noexcept {
        return data_;
    }
private:
    struct MethodArgsData {
        std::vector<Value> values;

        template <size_t N, typename... TArgs>
        void call(TArgs&&... args) {
            values.reserve(sizeof...(args));
            _call<N>(std::forward<TArgs>(args)...);
        }
    private:
        template <size_t I>
        constexpr void _call() {
            static_assert(I == 0);
        }

        template <size_t I, typename T, typename... TArgs>
        constexpr void _call(T &&arg, TArgs&&... args) {
            values.emplace_back(std::forward<T>(arg));
            _call<I - 1>(std::forward<TArgs>(args)...);
        }
    };

};

}

#endif // XMLRPC_XMLRPC_HPP
