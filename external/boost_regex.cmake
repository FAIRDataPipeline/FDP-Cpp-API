message(STATUS "[Boost Regex]")

set(BOOST_ENABLE_CMAKE ON CACHE INTERNAL "")

find_package(Boost COMPONENTS regex)
if(NOT Boost_FOUND)
    set(BRX_URL "https://github.com/boostorg/regex/archive/refs/tags/boost-1.79.0.zip")

    message(STATUS "\tBoost Regex Will be installed.")
    message(STATUS "\tURL: ${BRX_URL}")

    set(BOOST_REGEX_STANDALONE ON CACHE INTERNAL "Get just regex from Boost libraries")
    FetchContent_Declare(
        boost_regex
        URL ${BRX_URL}
    )
    FetchContent_MakeAvailable(boost_regex)
    
    message(STATUS "WARNING: Cannot set install targets for a static library using standalone Boost regex.")
    message(STATUS "         You will not be able to install this project.")
    message(STATUS "         Either install Boost to your system, or build with -DBUILD_SHARED_LIBS=ON.")
    SET(FDPAPI_WITH_INSTALL OFF CACHE BOOL "Override option" FORCE)
endif()
