#include "constants.hpp"

#include <boost/test/unit_test.hpp>

#include <subdl/metadata.hpp>

using namespace subdl;

BOOST_AUTO_TEST_SUITE(metadata)

BOOST_AUTO_TEST_CASE(non_existing_path) {
    MetaDataLoader loader;
    auto metadata = loader.get_metadata(NON_EXISTING_PATH);
    BOOST_CHECK(!metadata.has_value());
}

BOOST_AUTO_TEST_CASE(nonvideofile) {
    MetaDataLoader loader;
    auto metadata = loader.get_metadata(REF1_PATH);
    BOOST_CHECK(!metadata.has_value());
}

BOOST_AUTO_TEST_CASE(testsrc) {
    MetaDataLoader loader;
    auto metadata = loader.get_metadata(TESTSRC_PATH);
    BOOST_REQUIRE(metadata.has_value());
    BOOST_CHECK_EQUAL(metadata->time_ms, 30000);
    BOOST_CHECK_EQUAL(metadata->fps, 25);
    BOOST_CHECK_EQUAL(metadata->frame_count, 25 * 30);
}

BOOST_AUTO_TEST_SUITE_END()
