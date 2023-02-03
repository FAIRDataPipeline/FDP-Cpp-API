message(STATUS "[JsonCPP]")

set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "Don't build json-cpp tests")

option(
    FDPAPI_FETCH_JSONCPP_ONLY
    "Don't use pre-installed JsonCpp. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_JSONCPP_NEVER
    "Only use pre-installed JsonCpp. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_JSONCPP_ONLY AND FDPAPI_FETCH_JSONCPP_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_JSONCPP_ONLY and FDPAPI_FETCH_JSONCPP_NEVER are mutually exclusive"
    )
endif()

if(NOT FDPAPI_FETCH_JSONCPP_ONLY)
    find_package(jsoncpp QUIET)
endif()

if(jsoncpp_FOUND AND NOT FDPAPI_FETCH_JSONCPP_ONLY)
    message(STATUS "\tJsonCpp found.")
elseif(NOT FDPAPI_FETCH_JSONCPP_NEVER)
    set(JSONCPP_URL "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.5.zip")

    message(STATUS "\tJsonCpp not found.")
    message(STATUS "\tJsonCpp will be installed.")
    message(STATUS "\tURL: ${JSONCPP_URL}")

    FetchContent_Declare(
        JsonCpp
        URL ${JSONCPP_URL}
    )
    FetchContent_MakeAvailable(JsonCpp)
else()
    message(FATAL_ERROR "\tJsonCpp not found.")
endif()
