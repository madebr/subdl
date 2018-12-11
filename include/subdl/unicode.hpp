#ifndef SUBDL_UNICODE_HPP
#define SUBDL_UNICODE_HPP

#include <string>
#include <string_view>

namespace subdl {

[[nodiscard]] std::wstring to_wstring(const std::string_view s) noexcept;
[[nodiscard]] std::string to_string(const std::wstring_view s) noexcept;

}

#endif // SUBDL_UNICODE_HPP
