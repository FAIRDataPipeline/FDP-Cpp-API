message(STATUS "[RE2]")

set(RE2_ENABLE_TESTING OFF CACHE INTERNAL "")

set(RE2_URL "https://github.com/google/re2.git")
set(RE2_COMMIT "2022-12-01")

message(STATUS "\tre2 (regex library) will be installed.")
message(STATUS "\tURL: ${RE2_URL}")
message(STATUS "\tCOMMIT: ${RE2_COMMIT}")

FetchContent_Declare(
    RE2
    GIT_REPOSITORY ${RE2_URL}
    GIT_TAG ${RE2_COMMIT}
)
FetchContent_MakeAvailable(RE2)
