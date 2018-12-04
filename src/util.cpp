#include "util.hpp"

#include <iostream>

namespace subdl {

BinaryToHex::BinaryToHex(const std::string_view &v) noexcept
    : v_(v) {
}

std::ostream &operator <<(std::ostream &os, const BinaryToHex &d) noexcept {
    for (auto c : d.v_) {
        os.fill('0');
        os.width(2);
        os << std::hex << static_cast<std::uint32_t>(static_cast<std::uint8_t>(c));
    }
    return os;
}
}
