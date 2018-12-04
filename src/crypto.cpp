#include <subdl/crypto.hpp>

#include <openssl/md5.h>
#include <openssl/evp.h>

#include <iostream>
#include <sstream>

namespace subdl {

static const EVP_MD *hash_to_md(hash_t h) noexcept {
    const EVP_MD *(*evp_md)();
    switch(h) {
    case hash_t::md2:
        evp_md = EVP_md2;
        break;
    case hash_t::md4:
        evp_md = EVP_md4;
        break;
    case hash_t::md5:
        evp_md = EVP_md5;
        break;
    case hash_t::sha1:
        evp_md =EVP_sha1;
        break;
    case hash_t::sha224:
        evp_md = EVP_sha224;
        break;
    case hash_t::sha256:
        evp_md = EVP_sha256;
        break;
    case hash_t::sha384:
        evp_md = EVP_sha384;
        break;
    default: [[fallthrough]]
    case hash_t::sha512:
        evp_md = EVP_sha512;
        break;
    }
    return evp_md();
}

Hasher::Hasher(hash_t h) noexcept {
    mdctx_ = EVP_MD_CTX_create();
    if (EVP_MD_CTX_init(static_cast<EVP_MD_CTX *>(mdctx_)) != 1) {
//      FIXME: signal error
    }
    change_hash(h);
}

Hasher::~Hasher() {
    EVP_MD_CTX_destroy(static_cast<EVP_MD_CTX *>(mdctx_));
}

void Hasher::reset() noexcept {
    if (EVP_DigestInit_ex(static_cast<EVP_MD_CTX *>(mdctx_), static_cast<const EVP_MD *>(md_), nullptr) != 1) {
//      FIXME: signal error
    }
}

void Hasher::change_hash(hash_t h) noexcept {
    md_ = hash_to_md(h);
    reset();
}
void Hasher::update(std::string_view data) noexcept {
    if (EVP_DigestUpdate(static_cast<EVP_MD_CTX *>(mdctx_), data.data(), data.size()) != 1) {
//      FIXME: signal error
    }
}
Hasher::max_md_t Hasher::finalize_reset() noexcept {
    max_md_t max_md;
    if (EVP_DigestFinal_ex(static_cast<EVP_MD_CTX *>(mdctx_), max_md.data.data(), &max_md.size) != 1) {
//      FIXME: signal error
    }
    reset();
    return max_md;
}

static_assert(Hasher::max_md_t::MAX_MD_SIZE >= EVP_MAX_MD_SIZE);

std::optional<Hasher::max_md_t> Hasher::max_md_t::from_string(std::string_view v) noexcept {
    if (v.size() > 2 * Hasher::max_md_t::MAX_MD_SIZE) {
        return std::nullopt;
    }
    if (v.size() % 2 != 0) {
        return std::nullopt;
    }
    Hasher::max_md_t res;
    const auto nb = v.size() / 2;
    res.size = nb;
    std::istringstream iss;
    iss >> std::hex;
    for (std::size_t i = 0; i < nb; ++i) {
        iss.str(std::string{v.substr(2*i, 2)});
        iss.seekg(0);
        unsigned v = 0;
        iss >> v;
        if (iss.fail()) {
            return std::nullopt;
        }
        res.data[i] = static_cast<std::uint8_t>(v);
    }
    return res;
}

std::string Hasher::max_md_t::to_string() const noexcept {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}


std::ostream &operator<<(std::ostream &os, const Hasher::max_md_t &md) noexcept {
    for (std::size_t i = 0; i < md.size; ++i) {
        os.fill('0');
        os.width(2);
        os << std::hex << static_cast<std::uint32_t>(static_cast<std::uint8_t>(md.data[i]));
    }
    return os;
}

bool Hasher::max_md_t::operator==(const max_md_t &other) const noexcept {
    if (size != other.size) {
        return false;
    }
    for (std::size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

}
