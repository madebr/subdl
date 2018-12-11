#ifndef SUBDL_METADATA_HPP
#define SUBDL_METADATA_HPP

#include <cstdint>
#include <memory>
#include <optional>

namespace subdl {

struct VideoMetaData {
    std::uint32_t time_ms;
    std::uint32_t frame_count;
    float fps;
};


class MetaDataLoader {
public:
    MetaDataLoader() noexcept;
    MetaDataLoader(const MetaDataLoader &) = delete;
    MetaDataLoader(MetaDataLoader &&) = default;
    ~MetaDataLoader();
    [[nodiscard]] std::optional<VideoMetaData> get_metadata(const std::string &filename) noexcept;
private:
    [[nodiscard]] std::optional<std::uint32_t> get_time_ms() noexcept;
    [[nodiscard]] std::optional<std::uint32_t> get_framecount() noexcept;
    std::optional<float> get_framerate() noexcept;
    struct Impl;
    std::unique_ptr<Impl> state_;
};

std::optional<VideoMetaData> extract_metadata(const std::string &filename);

}

#endif // SUBDL_METADATA_HPP
