find_path(ICU_INCLUDE_PATH
    MediaInfo.h
    PATH_SUFFIXES MediaInfo
    DOC "libicu include path"
    )

find_library(ICU_DATA_LIBRARY
    DOC "Location of libicu data library"
    icudata
)

find_library(ICU_I18N_LIBRARY
    DOC "Location of libicu i18n library"
    icui18n
)

find_library(ICU_IO_LIBRARY
    DOC "Location of libicu io library"
    icuio
)

find_library(ICU_TU_LIBRARY
    DOC "Location of libicu tu library"
    icutu
)

find_library(ICU_UC_LIBRARY
    DOC "Location of libicu uc library"
    icuuc
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MediaInfo
    REQUIRED_VARS ICU_INCLUDE_PATH ICU_DATA_LIBRARY ICU_I18N_LIBRARY ICU_IO_LIBRARY
)

include(CheckTypeSize)
check_type_size(wchar_t SIZE_WCHAR_T)

add_library(icu INTERFACE IMPORTED)
set_target_properties(icu
    PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${ICU_INCLUDE_PATH}"
        INTERFACE_LINK_LIBRARIES "${ICU_DATA_LIBRARY};${ICU_I18N_LIBRARY};${ICU_IO_LIBRARY};${ICU_TU_LIBRARY};${ICU_UC_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "U_SIZEOF_WCHAR_T=${SIZE_WCHAR_T};U_CHARSET_IS_UTF8=1"
)
