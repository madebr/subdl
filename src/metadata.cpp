#include <subdl/metadata.hpp>

#include <subdl/unicode.hpp>
#include <subdl/version.hpp>

#include <MediaInfo/MediaInfo.h>

#include <optional>
#include <sstream>

namespace subdl {

#define MEDIAINFO_KNOWN_VERSION "18.08.1"

namespace ml = MediaInfoLib;

[[nodiscard]]
static ml::String to_mediainfo_string(const std::string_view &s) {
    return to_wstring(s);
}

[[nodiscard]]
static ml::String to_mediainfo_string(const std::wstring_view &s) {
    return static_cast<MediaInfoLib::String>(s);
}

template<typename T>
std::optional<T> extract_number(const ml::String &s) {
    ml::tiStringStream iss(s);
    T res;
    iss >> res;
    if (iss.fail()) {
        return std::nullopt;
    }
    return res;
}

struct MetaDataLoader::Impl {
    ml::MediaInfo mi;
    bool init() noexcept {
        auto res = mi.Option(__T("Internet"), __T("No"));
        {
            std::ostringstream oss;
            oss << MEDIAINFO_KNOWN_VERSION << ';' << SUBDL_NAME << ';' << SUBDL_VERSION;
            auto info_version = oss.str();
            auto ret = ml::MediaInfo::Option_Static(__T("Info_Version"), to_mediainfo_string(info_version));
            if (!ret.length()) {
                // FIXME: exit and require MEDIAINFO_KNOWN_VERSION
                return false;
            }
        }
        return true;
    }
};

MetaDataLoader::MetaDataLoader() noexcept
    : state_{std::make_unique<Impl>()} {

    state_->init();
}

MetaDataLoader::~MetaDataLoader() = default;

std::optional<VideoMetaData> MetaDataLoader::get_metadata(const std::string &filename) noexcept {
    auto opened = state_->mi.Open(to_mediainfo_string(filename));
    if (!opened) {
        return std::nullopt;
    }
    auto time_ms = get_time_ms();
    auto framecount = get_framecount();
    auto framerate = get_framerate();
    if (!time_ms || !framecount || !framerate) {
        return std::nullopt;
    }
    return VideoMetaData {
        *time_ms,
        *framecount,
        *framerate,
    };
}

std::optional<std::uint32_t> MetaDataLoader::get_time_ms() noexcept {
    std::optional<std::uint32_t> time_ms;
    auto streams_nb = state_->mi.Count_Get(ml::Stream_General);
    for (size_t stream_number = 0; stream_number < streams_nb; ++stream_number) {
        auto streamLength_str = state_->mi.Get(ml::Stream_General, stream_number, __T("Duration"));
        if (streamLength_str.length() == 0) {
            return std::nullopt;
        }
        auto stream_ms = extract_number<std::uint32_t>(streamLength_str);
        if (!stream_ms) {
            return std::nullopt;
        }
        if (time_ms) {
            time_ms = *time_ms + *stream_ms;
        } else {
            time_ms = stream_ms;
        }
    }
    return time_ms;
}

std::optional<std::uint32_t> MetaDataLoader::get_framecount() noexcept {
    std::optional<std::uint32_t> frameCount;
    auto streams_nb = state_->mi.Count_Get(ml::Stream_Video);
    for (size_t stream_number = 0; stream_number < streams_nb; ++stream_number) {
        auto frameCount_str = state_->mi.Get(ml::Stream_Video, stream_number, __T("FrameCount"));
        if (frameCount_str.length() == 0) {
            return std::nullopt;
        }
        auto streamFrameCount = extract_number<std::uint32_t>(frameCount_str);
        if (!streamFrameCount) {
            return std::nullopt;
        }
        if (frameCount) {
            frameCount = *frameCount + *streamFrameCount;
        } else {
            frameCount = streamFrameCount;
        }
    }
    return frameCount;
}

std::optional<float> MetaDataLoader::get_framerate() noexcept {
    std::optional<float> framerate;
    auto streams_nb = state_->mi.Count_Get(ml::Stream_Video);
    for (size_t stream_number = 0; stream_number < streams_nb; ++stream_number) {
        auto streamframerate_str = state_->mi.Get(ml::Stream_Video, stream_number, __T("FrameRate"));
        if (streamframerate_str.length() == 0) {
            return std::nullopt;
        }
        auto streamFramerate = extract_number<float>(streamframerate_str);
        if (!streamFramerate) {
            return std::nullopt;
        }
        if (framerate) {
            if (streamFramerate > framerate) {
                framerate = streamFramerate;
            }
        } else {
            framerate= streamFramerate;
        }
    }
    return framerate;
}

}
