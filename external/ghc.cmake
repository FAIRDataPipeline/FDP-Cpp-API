message(STATUS "[GHC]")

set(GHC_FILESYSTEM_WITH_INSTALL ON CACHE INTERNAL "Create import targets for ghc-filesystem")

option(
    FDPAPI_FETCH_GHC_ONLY
    "Don't use pre-installed ghc-filesystem. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_GHC_NEVER
    "Only use pre-installed ghc-filesystem. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_GHC_ONLY AND FDPAPI_FETCH_GHC_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_GHC_ONLY and FDPAPI_FETCH_GHC_NEVER are mutually exclusive"
    )
endif()

if(NOT (FDPAPI_FETCH_GHC_ONLY OR FDPAPI_FETCH_ONLY))
    find_package(ghc_filesystem QUIET)
endif()

if(ghc_filesystem_FOUND AND NOT (FDPAPI_FETCH_GHC_ONLY OR FDPAPI_FETCH_ONLY))
    message(STATUS "\tGHC found.")
elseif(NOT (FDPAPI_FETCH_GHC_NEVER OR FDPAPI_FETCH_NEVER))
    set(GHC_URL "https://github.com/gulrak/filesystem/archive/refs/tags/v1.5.10.zip")

    message(STATUS "\tGHC not found.")
    message(STATUS "\tGHC will be installed.")
    message(STATUS "\tURL: ${GHC_URL}")

    FetchContent_Declare(
        GHC
        URL ${GHC_URL}
    )
    FetchContent_MakeAvailable(GHC)
else()
    message(FATAL_ERROR "\tGHC not found.")
endif()
