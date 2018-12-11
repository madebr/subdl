
#include <cstdint>
#include <iostream>

#include <functional>
#include <map>
#include <variant>
#include <vector>
#include <string>

#include <iostream>

//template <typename... Args>
//void f(Args&& ...args);

struct FF {
    static void f() {
    }
    template <typename... Args>
    static void f(int, Args&& ...args) {
        std::cout << " int";
        f(std::forward<Args>(args)...);
    }
    template <typename... Args>
    static void f(bool, Args&& ...args) {
        std::cout << " bool";
        f(std::forward<Args>(args)...);
    }
    template <typename... Args>
    static void f(const char*, Args&& ...args) {
        std::cout << " char*";
        f(std::forward<Args>(args)...);
    }
    template <typename... Args>
    static void f(const std::string &, Args&& ...args) {
        std::cout << " string";
        f(std::forward<Args>(args)...);
    }
    template <typename... Args>
    static void myfun(Args&& ...args) {
        f(std::forward<Args>(args)...);
        std::cout << '\n';
    }
};

void test() {
    FF::myfun(static_cast<bool>(true), 1, "", "");
    FF::myfun(4, true, "", "", "", "");
}
void main_test_build() {
    test();
}
