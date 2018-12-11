find_library(MEDIAINFO_LIBRARY
    DOC "Location of libmediainfo library"
    mediainfo
)

find_path(MEDIAINFO_INCLUDE_PATH
    MediaInfo.h
    PATH_SUFFIXES MediaInfo
    DOC "Location of libmediainfo include path"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MediaInfo
    REQUIRED_VARS MEDIAINFO_LIBRARY MEDIAINFO_INCLUDE_PATH
)

if(NOT TARGET MediaInfo::MediaInfo)
    add_library(MediaInfo::MediaInfo INTERFACE IMPORTED)
    set_target_properties(MediaInfo::MediaInfo
        PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${MEDIAINFO_INCLUDE_PATH}"
            INTERFACE_LINK_LIBRARIES "${MEDIAINFO_LIBRARY}"
            INTERFACE_COMPILE_DEFINITIONS "_UNICODE"
    )
endif()
