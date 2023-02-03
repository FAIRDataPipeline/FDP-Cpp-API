message(STATUS "[RE2]")

set(RE2_BUILD_TESTING OFF CACHE INTERNAL "")

option(
    FDPAPI_FETCH_RE2_ONLY
    "Don't use pre-installed RE2. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_RE2_NEVER
    "Only use pre-installed RE2. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_RE2_ONLY AND FDPAPI_FETCH_RE2_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_RE2_ONLY and FDPAPI_FETCH_RE2_NEVER are mutually exclusive"
    )
endif()

if(NOT FDPAPI_FETCH_RE2_ONLY)
    find_package(re2 QUIET)
endif()

if(re2_FOUND AND NOT FDPAPI_FETCH_RE2_ONLY)
    message(STATUS "\tRE2 found.")
elseif(NOT FDPAPI_FETCH_RE2_NEVER)
    set(RE2_URL "https://github.com/google/re2.git")
    set(RE2_COMMIT "2022-12-01")

    message(STATUS "\tRE2 not found.")
    message(STATUS "\tRE2 will be installed.")
    message(STATUS "\tURL: ${RE2_URL}")
    message(STATUS "\tCOMMIT: ${RE2_COMMIT}")

    FetchContent_Declare(
        RE2
        GIT_REPOSITORY ${RE2_URL}
        GIT_TAG ${RE2_COMMIT}
    )
    FetchContent_MakeAvailable(RE2)
else()
    message(FATAL_ERROR "\tRE2 not found.")
endif()
