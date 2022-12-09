set(DIGESTCPP_URL "https://github.com/LiamPattinson/digestpp.git")
set(DIGESTCPP_COMMIT  "6f6f134")

message(STATUS "[DigestCPP]" )

message(STATUS "\tDigestCpp Will be installed.")
message(STATUS "\t(Using a fork with a CMake build system)")
message(STATUS "\tURL: ${DIGESTCPP_URL}")
message(STATUS "\tCOMMIT HASH: ${DIGESTCPP_COMMIT}")

# windows.h will conflict with min functions in digestcpp
# Because of macro definitions of min and max
# So tell the compiler to exclude min and max macros in windows
if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-DNOGDI)
endif()

FetchContent_Declare(
    DIGESTPP
    GIT_REPOSITORY ${DIGESTCPP_URL}
    GIT_TAG ${DIGESTCPP_COMMIT}
)
FetchContent_MakeAvailable(DIGESTPP)
