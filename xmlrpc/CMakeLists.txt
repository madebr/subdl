cmake_minimum_required(VERSION 3.10)
project(xmlrpc VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(xmlrpc
    src/types.cpp
    include/xmlrpc/types.hpp

    src/xmlrpc.cpp
    include/xmlrpc/xmlrpc.hpp
)

target_include_directories(xmlrpc
    PUBLIC
        "$<BUILD_INTERFACE:${xmlrpc_SOURCE_DIR}/include>"
)

target_compile_options(xmlrpc
    PRIVATE
        -Wall
        -Wpedantic
)

option(XMLRPC_TESTS "Build xmlrpc tests")
option(XMLRPC_RAPIDXML "Build rapidXml support")

if(XMLRPC_TESTS)
    enable_testing()
    find_package(Boost REQUIRED unit_test_framework)

    add_executable(xmlrpc_tests
        test/main.cpp
        test/types.cpp
        test/xmlrpc.cpp
    )
    target_link_libraries(xmlrpc_tests
        PUBLIC
            xmlrpc
            Boost::unit_test_framework
    )
    target_compile_definitions(xmlrpc_tests
        PRIVATE
            BOOST_ALL_DYN_LINK
    )
    add_test(NAME xmlrpc_tests
        COMMAND xmlrpc_tests
    )
endif()

if(XMLRPC_RAPIDXML)
    find_package(RapidXml REQUIRED)

    add_library(xmlrpc_rapidxml
        src/rapidxml.cpp
        include/xmlrpc/rapidxml.hpp
    )

    target_link_libraries(xmlrpc_rapidxml
        PUBLIC
            xmlrpc
    )
    if(XMLRPC_TESTS)
        target_sources(xmlrpc_tests
            PUBLIC
            test/rapidxml.cpp
        )
        target_link_libraries(xmlrpc_tests
            PUBLIC
                xmlrpc_rapidxml
        )
    endif()
    coverage_add_target(xmlrpc_rapidxml)
endif()

if(XMLRPC_TESTS)
    coverage_add_target(xmlrpc_tests)
endif()

coverage_add_target(xmlrpc)
