#include <boost/test/unit_test.hpp>

#include <subdl/crypto.hpp>

#include <string_view>

using namespace subdl;


BOOST_AUTO_TEST_SUITE(crypto_max_md)

BOOST_AUTO_TEST_CASE(from_string_good) {
    std::string_view hexstring = "0123456789abcdef";
    std::array<std::uint8_t, 8> refdata = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    BOOST_REQUIRE_EQUAL(refdata.size(), hexstring.size() / 2);

    auto res = Hasher::max_md_t::from_string(hexstring);
    BOOST_REQUIRE(res.has_value());
    BOOST_REQUIRE_EQUAL(refdata.size(), res->size);
    for (size_t i = 0; i < refdata.size(); ++i) {
        BOOST_CHECK_EQUAL(refdata[i], res->data[i]);
    }
}

BOOST_AUTO_TEST_CASE(from_string_bad) {
    {
        std::string_view hexstring_uneven = "012";
        auto res = Hasher::max_md_t::from_string(hexstring_uneven);
        BOOST_REQUIRE(!res.has_value());
    }
    {
        std::string_view hexstring_illegalchar = "xfde";
        auto res = Hasher::max_md_t::from_string(hexstring_illegalchar);
        BOOST_REQUIRE(!res.has_value());
    }
}

BOOST_AUTO_TEST_CASE(to_string) {
    std::string_view hexstring = "deadbeef";
    auto max_md = Hasher::max_md_t::from_string(hexstring);
    std::string res = max_md->to_string();
    BOOST_CHECK_EQUAL(hexstring, res);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(crypto)

const std::array<std::tuple<std::string_view, std::string_view>, 7> md5_refs = {
    std::make_tuple("",
                    "d41d8cd98f00b204e9800998ecf8427e"),
    std::make_tuple("a",
                    "0cc175b9c0f1b6a831c399e269772661"),
    std::make_tuple("abc",
                    "900150983cd24fb0d6963f7d28e17f72"),
    std::make_tuple("message digest",
                    "f96b697d7cb7938d525a2f31aaf161d0"),
    std::make_tuple("abcdefghijklmnopqrstuvwxyz",
                    "c3fcd3d76192e4007dfb496cca67e13b"),
    std::make_tuple("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
                    "d174ab98d277d9f5a5611c2c9f419d9f"),
    std::make_tuple("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
                    "57edf4a22be3c955ac49da2e2107b67a"),
};

BOOST_AUTO_TEST_CASE(md5) {
    Hasher hasher{hash_t::md5};
    for (const auto &md5_ref : md5_refs) {
        auto [data, refmd_hexstring] = md5_ref;
        auto refmd = Hasher::max_md_t::from_string(refmd_hexstring);
        BOOST_REQUIRE(refmd.has_value());
        hasher.update(data);
        auto md = hasher.finalize_reset();
        BOOST_CHECK_EQUAL(*refmd, md);
    }
}

BOOST_AUTO_TEST_SUITE_END()
