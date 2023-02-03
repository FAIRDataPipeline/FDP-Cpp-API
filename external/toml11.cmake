message(STATUS "[TOML 11]")

option(
    FDPAPI_FETCH_TOML11_ONLY
    "Don't use pre-installed TOML11. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_TOML11_NEVER
    "Only use pre-installed TOML 11. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_TOML11_ONLY AND FDPAPI_FETCH_TOML11_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_TOML11_ONLY and FDPAPI_FETCH_TOML11_NEVER are mutually exclusive"
    )
endif()

if(NOT (FDPAPI_FETCH_TOML11_ONLY OR FDPAPI_FETCH_ONLY))
    find_package(toml11 QUIET)
endif()


if(toml11_FOUND AND NOT (FDPAPI_FETCH_TOML11_ONLY OR FDPAPI_FETCH_ONLY))
    message(STATUS "\tTOML11 found.")
elseif(NOT (FDPAPI_FETCH_TOML11_NEVER OR FDPAPI_FETCH_NEVER))
    set( TOML11_URL "https://github.com/ToruNiina/toml11/archive/refs/tags/v3.7.0.zip" )

    message(STATUS "\tTOML 11 not found.")
    message(STATUS "\tTOML 11 will be installed.")
    message(STATUS "\tURL: ${TOML11_URL}")

    FetchContent_Declare(
        toml11
        URL ${TOML11_URL}
    )
    FetchContent_MakeAvailable(toml11)
else()
    message(FATAL_ERROR "\tTOML 11 not found.")
endif()
