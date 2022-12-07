set(DIGESTCPP_URL "https://github.com/LiamPattinson/digestpp.git")
set(DIGESTCPP_COMMIT  "422279e")

message(STATUS "[DigestCPP]" )

message(STATUS "\tDigestCpp Will be installed.")
message(STATUS "\t(Using a fork with a CMake build system)")
message(STATUS "\tURL: ${DIGESTCPP_URL}")
message(STATUS "\tCOMMIT HASH: ${DIGESTCPP_COMMIT}")

FetchContent_Declare(
    DIGESTPP
    GIT_REPOSITORY ${DIGESTCPP_URL}
    GIT_TAG ${DIGESTCPP_COMMIT}
)
FetchContent_MakeAvailable(DIGESTPP)
