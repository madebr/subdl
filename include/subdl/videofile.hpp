#ifndef SUBDL_VIDEOFILE_HPP
#define SUBDL_VIDEOFILE_HPP

#include "metadata.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <iosfwd>

namespace subdl {

struct OsdbHashType {
    std::uint64_t val;
    bool operator==(const OsdbHashType &other) const noexcept;
    friend std::ostream &operator <<(std::ostream &os, const OsdbHashType & h) noexcept;
};

class VideoFile {
public:
    static std::optional<VideoFile> from_path(const std::string &path, MetaDataLoader &loader) noexcept;
    const std::string &path() const noexcept;
    std::uint64_t filesize() const noexcept;
    OsdbHashType osdb_hash() const noexcept;
    const std::optional<VideoMetaData> &video_metadata() const noexcept;
private:
    VideoFile(const std::string &path, MetaDataLoader &loader) noexcept;
    void read(MetaDataLoader &loader) noexcept;
    std::string path_;
    std::uint64_t filesize_{};
    OsdbHashType osdb_hash_{};
    std::optional<VideoMetaData> metadata_;
};

std::optional<std::uint64_t> read_filesize(const std::string &path) noexcept;
std::optional<OsdbHashType> read_osdb_hash(const std::string &s) noexcept;

}

#endif // SUBDL_VIDEOFILE_HPP
