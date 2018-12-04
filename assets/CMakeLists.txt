set(CACHE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/cache")

set(INTERMEDIATE_COUNTRY_CODES "${CACHE_FOLDER}/country_codes.csv")
add_custom_command(
    OUTPUT "${INTERMEDIATE_COUNTRY_CODES}"
    COMMAND "${PYTHON_EXECUTABLE}" "generate_country_codes.py" fetch -o "${INTERMEDIATE_COUNTRY_CODES}"
    DEPENDS "generate_country_codes.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(INTERMEDIATE_IANA_TAGS "${CACHE_FOLDER}/language-subtag-registry.txt")
add_custom_command(
    OUTPUT "${INTERMEDIATE_IANA_TAGS}"
    COMMAND "${PYTHON_EXECUTABLE}" "generate_iana_tags.py" fetch -o "${INTERMEDIATE_IANA_TAGS}"
    DEPENDS "generate_iana_tags.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(INTERMEDIATE_ISO639_1_2 "${CACHE_FOLDER}/iso639-1_2.html")
add_custom_command(
    OUTPUT "${INTERMEDIATE_ISO639_1_2}"
    COMMAND "${PYTHON_EXECUTABLE}" "generate_iso639_1_2.py" fetch -o "${INTERMEDIATE_ISO639_1_2}"
    DEPENDS "generate_iso639_1_2.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(INTERMEDIATE_ISO639_3 "${CACHE_FOLDER}/iso639-3.zip")
add_custom_command(
    OUTPUT "${INTERMEDIATE_ISO639_3}"
    COMMAND "${PYTHON_EXECUTABLE}" "generate_iso639_3.py" fetch -o "${INTERMEDIATE_ISO639_3}"
    DEPENDS "generate_iso639_3.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(INTERMEDIATE_ISO639_5 "${CACHE_FOLDER}/iso639-5.tsv")
add_custom_command(
    OUTPUT "${INTERMEDIATE_ISO639_5}"
    COMMAND "${PYTHON_EXECUTABLE}" "generate_iso639_5.py" fetch -o "${INTERMEDIATE_ISO639_5}"
    DEPENDS "generate_iso639_5.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(update_assets_cache
    DEPENDS
        "${INTERMEDIATE_COUNTRY_CODES}"
        "${INTERMEDIATE_IANA_TAGS}"
        "${INTERMEDIATE_ISO639_1_2}"
        "${INTERMEDIATE_ISO639_3}"
        "${INTERMEDIATE_ISO639_5}"
)