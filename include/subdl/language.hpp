#ifndef SUBDL_LANGUAGE_HPP
#define SUBDL_LANGUAGE_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include <string>

namespace subdl {
class LanguageTag {
public:
    LanguageTag() noexcept;
    LanguageTag(std::string_view v) noexcept;
private:
    std::string code_ = {};
};

}

#endif // SUBDL_LANGUAGE_HPP
