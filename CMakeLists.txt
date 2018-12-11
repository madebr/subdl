cmake_minimum_required(VERSION 3.11)
project(subdl
    VERSION 0.1
    LANGUAGES C CXX
)

option(SUBDL_TESTS "Build subdl tests")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

include(CodeCoverage)
codecoverage_enable()

find_package(cpprestsdk REQUIRED CONFIG)
find_package(Boost REQUIRED COMPONENTS system)
find_package(fmt REQUIRED)
find_package(icu REQUIRED)
find_package(MediaInfo REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
find_package(RapidXml REQUIRED)

set(Python_ADDITIONAL_VERSIONS 3)
find_package(PythonInterp REQUIRED COMPONENTS Interpreter)
if (PYTHON_VERSION_STRING VERSION_LESS 3.4)
    message(FATAL_ERROR "Need Python 3.4 or higher. Found ${PYTHON_VERSION_STRING}")
endif()

add_subdirectory(assets)
add_subdirectory(external)

configure_file(include/subdl/version.hpp.in include/subdl/version.hpp)

include(TestBigEndian)
test_big_endian(ARCH_BIG_ENDIAN)

add_library(subdl
    include/subdl/version.hpp

    src/crypto.cpp
    include/subdl/crypto.hpp

    src/error.cpp
    include/subdl/error.hpp

    src/language.cpp
    include/subdl/language.hpp

    src/metadata.cpp
    include/subdl/metadata.hpp

    src/subtitle.cpp
    include/subdl/subtitle.hpp

    src/videofile.cpp
    include/subdl/videofile.hpp

    src/unicode.cpp
    include/subdl/unicode.hpp

    src/util.cpp
    src/util.hpp

    src/provider/opensubtitles.cpp
    src/provider/opensubtitles2.cpp
    src/provider/opensubtitles.hpp

    src/enumerate.hpp
)

target_include_directories(subdl
    PUBLIC
        include
        "${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_link_libraries(subdl
    PRIVATE
        Boost::boost
        Boost::system
        stdc++fs
        cpprestsdk::cpprest
        Boost::system
        fmt
        icu
        #iqxmlrpc
        MediaInfo::MediaInfo
        OpenSSL::Crypto
        ${CMAKE_DL_LIBS}
        xmlrpc_rapidxml
        RapidXml::RapidXml
)

target_compile_options(subdl
    PRIVATE
        -Wall
        -Wpedantic
)

target_compile_definitions(subdl
    PRIVATE
        "$<$<BOOL:${ARCH_BIG_ENDIAN}>:SUBDL_BIG_ENDIAN>"
)

set_target_properties(subdl
    PROPERTIES
        C_STANDARD 11
        C_EXTENSIONS OFF
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
)

if(SUBDL_TESTS)
    find_package(Boost REQUIRED COMPONENTS unit_test_framework system)

    add_executable(subdltests
        test/constants.hpp
        test/main.cpp
        test/crypto.cpp
        test/metadata.cpp
        test/videofile.cpp
    )

    target_compile_definitions(subdltests
        PRIVATE
            BOOST_ALL_DYN_LINK
    )

    target_link_libraries(subdltests
        PUBLIC
            subdl
            Boost::unit_test_framework
            stdc++fs
    )

    target_compile_options(subdltests
        PRIVATE
            -Wall
            -Wpedantic
    )

    set_target_properties(subdltests
        PROPERTIES
            C_STANDARD 11
            C_EXTENSIONS OFF
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
    )

    set(TEST_ASSETS ref1.bin testsrc.mp4 secret.bin)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test_assets")

    add_custom_command(TARGET subdltests
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy ${TEST_ASSETS} "${CMAKE_CURRENT_BINARY_DIR}/test_assets"
        COMMAND chmod a-rwx "${CMAKE_CURRENT_BINARY_DIR}/test_assets/secret.bin"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/assets"
    )

    enable_testing()
    add_test(NAME subdltests
        COMMAND subdltests
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    coverage_add_target(subdltests)
endif()

set(XMLRPC_RAPIDXML ON)
add_subdirectory(xmlrpc)

add_executable(main
    src/build.cpp  # FIXME: remove once finished
    src/main.cpp
)

target_link_libraries(main
    PUBLIC
        subdl
        xmlrpc_rapidxml
)

set_target_properties(main
    PROPERTIES
        C_STANDARD 11
        C_EXTENSIONS OFF
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
)

coverage_add_target(subdl)
coverage_add_target(main)

coverage_collect()
