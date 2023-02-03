message(STATUS "[Curl]")

option(
    FDPAPI_FETCH_CURL_ONLY
    "Don't use pre-installed curl. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_CURL_NEVER
    "Only use pre-installed curl. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_CURL_ONLY AND FDPAPI_FETCH_CURL_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_CURL_ONLY and FDPAPI_FETCH_CURL_NEVER are mutually exclusive"
    )
endif()

if(NOT (FDPAPI_FETCH_CURL_ONLY OR FDPAPI_FETCH_ONLY))
    find_package(CURL QUIET)
endif()


if(CURL_FOUND AND NOT (FDPAPI_FETCH_CURL_ONLY OR FDPAPI_FETCH_ONLY))
    message(STATUS "\tCURL found.")
elseif(NOT (FDPAPI_FETCH_CURL_NEVER OR FDPAPI_FETCH_NEVER))
    set(CURL_URL "https://github.com/curl/curl/archive/refs/tags/curl-7_80_0.zip")

    message(STATUS "\tCURL not found.")
    message(STATUS "\tInstalling from URL: ${CURL_URL}")

    FetchContent_Declare(
        CURL
        URL ${CURL_URL}
    )
    FetchContent_MakeAvailable(CURL)

    set(CURL_INCLUDE_DIRS = ${curl_SOURCE_DIR}/include)
    set(CURL_LIBRARIES libcurl)
else()
    message(FATAL_ERROR "\tCURL not found.")
endif()

message(STATUS "\tInclude Directory: ${CURL_INCLUDE_DIRS}")
message(STATUS "\tLibraries: ${CURL_LIBRARIES}")
