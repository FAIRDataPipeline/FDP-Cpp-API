MESSAGE( STATUS "[Boost Regex]" )

set(BRX_URL "https://github.com/boostorg/regex/archive/refs/tags/boost-1.79.0.zip")

MESSAGE( STATUS "\tBoost Regex Will be installed." )
MESSAGE( STATUS "\tURL: ${BRX_URL}" )

set(BOOST_ENABLE_CMAKE ON CACHE INTERNAL "")

find_package(Boost COMPONENTS regex)
if(NOT Boost_FOUND)
    set(BOOST_REGEX_STANDALONE ON CACHE INTERNAL "Get just regex from Boost libraries")
    FetchContent_Declare(
        boost_regex
        URL ${BRX_URL}
    )
    FetchContent_MakeAvailable(boost_regex)
endif()
