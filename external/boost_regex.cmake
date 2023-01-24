message(STATUS "[Boost Regex]")

set(BOOST_ENABLE_CMAKE ON CACHE INTERNAL "")
set(BOOST_REGEX_STANDALONE ON CACHE INTERNAL "Get just regex from Boost libraries")

find_package(Boost COMPONENTS regex)
if(NOT Boost_FOUND)
    set(BRX_URL "https://github.com/boostorg/regex/archive/refs/tags/boost-1.79.0.zip")

    message(STATUS "\tBoost Regex Will be installed.")
    message(STATUS "\tURL: ${BRX_URL}")

    FetchContent_Declare(
        BOOSTREGEX
        URL ${BRX_URL}
    )
    FetchContent_MakeAvailable(BOOSTREGEX)
endif()
