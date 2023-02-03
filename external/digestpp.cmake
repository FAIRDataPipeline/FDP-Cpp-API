message(STATUS "[digestpp]" )

option(
    FDPAPI_FETCH_DIGESTPP_ONLY
    "Don't use pre-installed digestpp. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_DIGESTPP_NEVER
    "Only use pre-installed digestpp. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_DIGESTPP_ONLY AND FDPAPI_FETCH_DIGESTPP_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_DIGESTPP_ONLY and FDPAPI_FETCH_DIGESTPP_NEVER are mutually exclusive"
    )
endif()

if(NOT (FDPAPI_FETCH_DIGESTPP_ONLY OR FDPAPI_FETCH_ONLY))
    find_package(digestpp QUIET)
endif()

if(digestpp_FOUND AND NOT (FDPAPI_FETCH_DIGESTPP_ONLY OR FDPAPI_FETCH_ONLY))
    message(STATUS "\tdigestpp found.")
elseif(NOT (FDPAPI_FETCH_DIGESTPP_NEVER OR FDPAPI_FETCH_NEVER))
    set(DIGESTPP_URL "https://github.com/LiamPattinson/digestpp.git")
    set(DIGESTPP_COMMIT  "6f6f134")

    message(STATUS "\tdigestpp not found.")
    message(STATUS "\tdigestpp will be installed.")
    message(STATUS "\t(Using a fork with a CMake build system)")
    message(STATUS "\tURL: ${DIGESTPP_URL}")
    message(STATUS "\tCOMMIT HASH: ${DIGESTPP_COMMIT}")

    FetchContent_Declare(
        DIGESTPP
        GIT_REPOSITORY ${DIGESTPP_URL}
        GIT_TAG ${DIGESTPP_COMMIT}
    )
    FetchContent_MakeAvailable(DIGESTPP)
else()
    message(FATAL_ERROR "\tdigestpp not found.")
endif()

# windows.h will conflict with min functions in digestcpp
# Because of macro definitions of min and max
# So tell the compiler to exclude min and max macros in windows
if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-DNOGDI)
endif()
