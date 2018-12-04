#include <subdl/unicode.hpp>

#include "unicode/unistr.h"

namespace subdl {

std::wstring to_wstring(const std::string_view s) noexcept {
    auto ucs = icu::UnicodeString::fromUTF8(icu::StringPiece(s.data(), s.length()));
    std::wstring result;
#if U_SIZEOF_WCHAR_T == 4
    result.resize(ucs.length());
    UErrorCode errorCode = U_ZERO_ERROR;
    ucs.toUTF32(reinterpret_cast<UChar32 *>(result.data()), result.capacity(), errorCode);
    if (U_FAILURE(errorCode)) {
        // FIXME: log error
//        std::cerr << "ERROR CONVERTING!!! (FIXME: print error)\n";
    }
#else
#error sizeof(wchar_t) != 4 is not supported
#endif
    return result;
}

std::string to_string(const std::wstring_view s) noexcept {
#if U_SIZEOF_WCHAR_T == 4
    auto ucs = icu::UnicodeString::fromUTF32(reinterpret_cast<const UChar32*>(s.data()), s.length());
    std::string result;
    result.resize(ucs.length());
    icu::StringByteSink<std::string> sink{&result};
    ucs.toUTF8(sink);
#else
#error sizeof(wchar_t) != 4 is not supported
#endif
    return result;
}

}
