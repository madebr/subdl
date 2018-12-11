#ifndef SUBDL_CRYPTO_HPP
#define SUBDL_CRYPTO_HPP

#include <array>
#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string_view>

namespace subdl {

enum class hash_t {
    md2,
    md4,
    md5,
    sha1,
    sha224,
    sha256,
    sha384,
    sha512,
};

class Hasher {
public:
    Hasher(hash_t h) noexcept;
    Hasher(Hasher &) noexcept = delete;
    Hasher(Hasher &&) noexcept = default;
    ~Hasher();

    struct max_md_t {
        static constexpr unsigned MAX_MD_SIZE = 64;
        std::uint32_t size;
        std::array<std::uint8_t, MAX_MD_SIZE> data;
        static std::optional<Hasher::max_md_t> from_string(std::string_view v) noexcept;
        std::string to_string() const noexcept;
        bool operator==(const max_md_t &other) const noexcept;
        friend std::ostream &operator<<(std::ostream &os, const max_md_t &md) noexcept;
    };

    void reset() noexcept;
    void change_hash(hash_t h) noexcept;
    void update(std::string_view data) noexcept;
    [[nodiscard]] max_md_t finalize_reset() noexcept;
private:
    void *mdctx_;
    const void *md_;
};

}

#endif // SUBDL_CRYPTO_HPP
