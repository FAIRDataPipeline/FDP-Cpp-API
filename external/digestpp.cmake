set(DIGESTCPP_URL "https://github.com/kerukuro/digestpp.git")
set(DIGESTCPP_COMMIT "34ff2eeae397ed744d972d86b5a20f603b029fbd")


# windows.h will conflict with min functions in digestcpp
# Because of macro definitions of min and max
# So tell the compiler to exclude min and max macros in windows
if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-DNOGDI)
endif()

message(STATUS "[DigestCPP]" )

message(STATUS "\tDigestCpp Will be installed.")
message(STATUS "\tURL: ${DIGESTCPP_URL}")
message(STATUS "\tCOMMIT HASH: ${DIGESTCPP_COMMIT}")

include(FetchContent)
FetchContent_Declare(
    DIGESTPP
    GIT_REPOSITORY ${DIGESTCPP_URL}
    GIT_TAG ${DIGESTCPP_COMMIT}
)
FetchContent_MakeAvailable(DIGESTPP)

add_library(digestpp INTERFACE)
add_library(digestpp::digestpp ALIAS digestpp)
target_include_directories(
    digestpp
    INTERFACE
    $<BUILD_INTERFACE:${digestpp_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Set rules for installing targets
set(digestpp_targets digestpp)
install(TARGETS ${digestpp_targets}
        EXPORT digestppTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export file, which contains code to allow other projects to import this project
install(
    EXPORT digestppTargets
    FILE digestppTargets.cmake
    NAMESPACE digestpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/digestpp
)

# Install this project's include dir, so projects importing it can access it
install(DIRECTORY ${digestpp_SOURCE_DIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
