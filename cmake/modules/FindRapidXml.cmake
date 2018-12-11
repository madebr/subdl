find_path(RAPIDXML_INCLUDE_PATH
    rapidxml.h
    DOC "Location of RapidXml include path"
)

if(RAPIDXML_INCLUDE_PATH)
    file(READ "${RAPIDXML_INCLUDE_PATH}/rapidxml.h" RAPIDXML_HEADER_STR)
    string(REGEX MATCH "Version ([0-9a-zA-Z\.]+)" _RAPIDXML_VERSION "${RAPIDXML_HEADER_STR}")
    if (_RAPIDXML_VERSION)
        set(RAPIDXML_VERSION "${CMAKE_MATCH_1}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidXml
    REQUIRED_VARS RAPIDXML_INCLUDE_PATH
    VERSION_VAR RAPIDXML_VERSION
)

if(NOT TARGET RapidXml::RapidXml)
    add_library(RapidXml::RapidXml INTERFACE IMPORTED)
    set_target_properties(RapidXml::RapidXml
        PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${RAPIDXML_INCLUDE_PATH}"
    )
endif()
