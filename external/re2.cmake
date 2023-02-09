set(RE2_BUILD_TESTING OFF CACHE INTERNAL "")
fdpapi_add_external(
    "RE2"
    REPO "https://github.com/google/re2.git"
    TAG "2022-12-01"
    PKG_NAME "re2"
)
