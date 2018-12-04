#include <subdl/language.hpp>

#include <algorithm>

namespace subdl {

LanguageTag::LanguageTag() noexcept = default;

LanguageTag::LanguageTag(std::string_view v) noexcept {
    code_.reserve(v.size());
    std::transform(v.begin(), v.end(), code_.begin(), ::tolower);
}

}
