#include "constants.hpp"

#include <boost/test/unit_test.hpp>

#include <subdl/videofile.hpp>

#include <filesystem>
#include <iostream>

using namespace subdl;

BOOST_AUTO_TEST_SUITE(videofile)

BOOST_AUTO_TEST_CASE(osdb_hash_equal) {
    OsdbHashType hash1{0xaa}, hash2{0xaa}, hash3{0x55};
    BOOST_CHECK_EQUAL(hash1, hash1);
    BOOST_CHECK_EQUAL(hash2, hash2);
    BOOST_CHECK_EQUAL(hash3, hash3);
    BOOST_CHECK_EQUAL(hash1, hash2);
    BOOST_CHECK_EQUAL(hash2, hash1);
    BOOST_CHECK_NE(hash1, hash3);
    BOOST_CHECK_NE(hash3, hash1);
    BOOST_CHECK_NE(hash2, hash3);
    BOOST_CHECK_NE(hash3, hash2);
}

BOOST_AUTO_TEST_CASE(osdb_hash_nonexisting) {
    auto osdb_hash = read_osdb_hash(NON_EXISTING_PATH);
    BOOST_CHECK(!osdb_hash.has_value());
}

BOOST_AUTO_TEST_CASE(osdb_hash_dir) {
    auto osdb_hash = read_osdb_hash(ASSET_DIR);
    BOOST_CHECK(!osdb_hash.has_value());
}

BOOST_AUTO_TEST_CASE(osdb_hash_good_ref1) {
    auto p = REF1_PATH;
    OsdbHashType h{0xe7f1a88fc3998dcd};
    BOOST_REQUIRE(std::filesystem::exists(p));
    auto osdb_hash = read_osdb_hash(p);
    BOOST_REQUIRE(osdb_hash.has_value());
    BOOST_CHECK_EQUAL(h, *osdb_hash);
}

BOOST_AUTO_TEST_CASE(osdb_hash_secret) {
    auto p = SECRET_PATH;
    BOOST_REQUIRE(std::filesystem::exists(p));
    auto osdb_hash = read_osdb_hash(p);
    BOOST_CHECK(!osdb_hash.has_value());
}

BOOST_AUTO_TEST_CASE(osdb_hash_good_testsrc) {
    auto p = TESTSRC_PATH;
    OsdbHashType h{0x6fd8d433d910112f};
    BOOST_REQUIRE(std::filesystem::exists(p));
    auto osdb_hash = read_osdb_hash(p);
    BOOST_REQUIRE(osdb_hash.has_value());
    BOOST_CHECK_EQUAL(h, *osdb_hash);
}

BOOST_AUTO_TEST_CASE(to_string) {
    {
        OsdbHashType h{0x0123456789abcdef};
        std::ostringstream oss;
        oss << h;
        auto hash_str = oss.str();
        BOOST_CHECK_EQUAL(hash_str, std::string{"0123456789abcdef"});
    }
    {
        OsdbHashType h{0x0};
        std::ostringstream oss;
        oss << h;
        auto hash_str = oss.str();
        BOOST_CHECK_EQUAL(hash_str, std::string{"0000000000000000"});
    }
}

BOOST_AUTO_TEST_CASE(filesize_nonexisting) {
    auto p = NON_EXISTING_PATH;
    auto filesize = read_filesize(p);
    BOOST_CHECK(!filesize.has_value());
}

BOOST_AUTO_TEST_CASE(filesize_existing) {
    auto p = REF1_PATH;
    auto filesize = read_filesize(p);
    BOOST_REQUIRE(filesize.has_value());
    BOOST_CHECK_EQUAL(2 * 64 << 10, *filesize);
}

BOOST_AUTO_TEST_CASE(videofile_nonexisting) {
    MetaDataLoader loader;
    auto  vf = VideoFile::from_path(NON_EXISTING_PATH, loader);
    BOOST_CHECK(!vf.has_value());
}

BOOST_AUTO_TEST_CASE(videofile_existing) {
    MetaDataLoader loader;
    auto  vf = VideoFile::from_path(TESTSRC_PATH, loader);
    BOOST_REQUIRE(vf.has_value());
    BOOST_CHECK_EQUAL(vf->path(), TESTSRC_PATH);
    BOOST_CHECK_NE(vf->osdb_hash(), OsdbHashType{0x0});
    BOOST_CHECK(vf->video_metadata().has_value());
    BOOST_CHECK_EQUAL(vf->filesize(), 138512);
}

BOOST_AUTO_TEST_SUITE_END()
