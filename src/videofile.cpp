#include <subdl/videofile.hpp>

#include "util.hpp"

#include <filesystem>
#include <iostream> // FIXME: remove
#include <fstream>

#include <byteswap.h>

namespace subdl {

bool OsdbHashType::operator ==(const OsdbHashType &other) const noexcept {
    return val == other.val;
}

std::ostream &operator <<(std::ostream &os, const OsdbHashType &h) noexcept {
    os.width(16);
    os.fill('0');
    return os << std::hex << h.val;
}

static std::optional<OsdbHashType> read_osdb_hash(const std::string &path, std::uint64_t filesize) noexcept;

std::optional<VideoFile> VideoFile::from_path(const std::string &path, MetaDataLoader &loader) noexcept {
    if (!std::filesystem::exists(path)) {
        return std::nullopt;
    }
    return VideoFile{path, loader};
}

VideoFile::VideoFile(const std::string &path, MetaDataLoader &loader) noexcept
    : path_(path) {
    read(loader);
}

void VideoFile::read(MetaDataLoader &loader) noexcept {
    std::error_code error_code;
    filesize_ = std::filesystem::file_size(path_, error_code);

    metadata_ = loader.get_metadata(path_);

    auto osdb_hash = read_osdb_hash(path_, filesize_);
    osdb_hash_ = *osdb_hash;
}

//bool VideoFile::good() const {
//    return good_;
//}

// FIXME: compare with reading to std::array and std::accumulate (c++20: std::span?)
static std::optional<OsdbHashType> read_osdb_hash(const std::string &path, std::uint64_t filesize) noexcept {
    OsdbHashType osdb_hash{filesize};
    static constexpr std::uint64_t OSDB_MAX_BLOCKSIZE = 64 << 10;
    auto const blocksize = std::min(filesize, OSDB_MAX_BLOCKSIZE);
    auto const nb_reads = blocksize / sizeof(OsdbHashType);

    std::ifstream ifs(path);
    ifs.seekg(0);
    for (size_t i = 0; i < nb_reads; ++i) {
        std::uint64_t var;
        ifs.read(reinterpret_cast<char *>(&var), sizeof(var));
#ifdef SUBDL_BIG_ENDIAN
        var = bswap_64(var);
#endif
        osdb_hash.val += var;
    }
    ifs.seekg(-static_cast<std::int64_t>(blocksize), std::ios_base::end);
    for (size_t i = 0; i < nb_reads; ++i) {
        std::uint64_t var;
        ifs.read(reinterpret_cast<char *>(&var), sizeof(var));
#ifdef SUBDL_BIG_ENDIAN
        var = bswap_64(var);
#endif
        osdb_hash.val += var;
    }
    if (ifs.fail()) {
        return std::nullopt;
    }
    return osdb_hash;
}

std::optional<std::uint64_t> read_filesize(const std::string &path) noexcept {
    try {
        return std::filesystem::file_size(path);
    } catch (const std::filesystem::filesystem_error &) {
        return std::nullopt;
    }
}

std::optional<OsdbHashType> read_osdb_hash(const std::string &path) noexcept {
    try {
        auto filesize = std::filesystem::file_size(path);
        return read_osdb_hash(path, filesize);
    } catch (const std::filesystem::filesystem_error &) {
        return std::nullopt;
    }
}

const std::string &VideoFile::path() const noexcept {
    return path_;
}

std::uint64_t VideoFile::filesize() const noexcept {
    return filesize_;
}

OsdbHashType VideoFile::osdb_hash() const noexcept {
    return osdb_hash_;
}

const std::optional<VideoMetaData> &VideoFile::video_metadata() const noexcept {
    return metadata_;
}

}
