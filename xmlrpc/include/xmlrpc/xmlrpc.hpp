#ifndef XMLRPC_XMLRPC_HPP
#define XMLRPC_XMLRPC_HPP

#include "types.hpp"

#include <string>
#include <vector>

namespace xmlrpc {

class MethodArgs {
    std::string name_;
public:
    template <typename T>
    MethodArgs(T &&name) : name_(std::forward<T>(name)) {
    }

    template <typename... TArgs>
    std::vector<Value> call(TArgs&&...args) {
        Args argsobj;
        argsobj.call<sizeof...(args)>(std::forward<TArgs>(args)...);
        return argsobj.values;
    }
private:
    struct Args {
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
//            values[I-1] = std::forward<T>(arg);
//            values[I-1] = std::forward<T>(arg);
            _call<I - 1>(std::forward<TArgs>(args)...);
        }
    };

};

}

#endif // XMLRPC_XMLRPC_HPP
