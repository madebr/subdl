#ifndef SUBDL_UTIL_HPP
#define SUBDL_UTIL_HPP

#include <array>
#include <cstdint>
#include <iosfwd>
#include <string_view>

namespace subdl {

class BinaryToHex {
    std::string_view v_;
public:
    BinaryToHex(const std::string_view &v) noexcept;
    template<std::size_t NB>
    BinaryToHex(const std::array<std::uint8_t, NB> &d) noexcept
        : v_(reinterpret_cast<const char *>(d.data()), d.size()) {
    }
    template<std::size_t NB>
    BinaryToHex(const std::array<std::uint8_t, NB> &d, std::size_t nb) noexcept
        : v_(reinterpret_cast<const char *>(d.data()), nb) {
    }
    friend std::ostream &operator <<(std::ostream &os, const BinaryToHex &d) noexcept;
};
}

#endif // SUBDL_UTIL_HPP
