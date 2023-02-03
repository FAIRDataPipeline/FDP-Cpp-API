message(STATUS "[YAML-cpp]")

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "Build yaml-cpp tests")
if(BUILD_SHARED_LIBS)
    set(YAML_BUILD_SHARED_LIBS ON CACHE INTERNAL "Build .so for yaml-cpp")
endif()
set(YAML_CPP_INSTALL ON CACHE INTERNAL "Include export targets for installation")

option(
    FDPAPI_FETCH_YAMLCPP_ONLY
    "Don't use pre-installed YAML-cpp. Recommended for testing only."
    OFF
)

option(
    FDPAPI_FETCH_YAMLCPP_NEVER
    "Only use pre-installed YAML-cpp. Recommended for testing only."
    OFF
)

if(FDPAPI_FETCH_YAMLCPP_ONLY AND FDPAPI_FETCH_YAMLCPP_NEVER)
    message(
        FATAL_ERROR
        "FDPAPI_FETCH_YAMLCPP_ONLY and FDPAPI_FETCH_YAMLCPP_NEVER are mutually exclusive"
    )
endif()

if(NOT FDPAPI_FETCH_YAMLCPP_ONLY)
    find_package(yaml-cpp QUIET)
endif()

if(yaml-cpp_FOUND AND NOT FDPAPI_FETCH_YAMLCPP_ONLY)
    message(STATUS "\tYAML-cpp found.")
elseif(NOT FDPAPI_FETCH_YAMLCPP_NEVER)
    set(YAML_CPP_GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git")
    set(YAML_CPP_GIT_TAG "1b50109f7bea60bd382d8ea7befce3d2bd67da5f")

    message(STATUS "\tYAML-cpp not found.")
    message(STATUS "\tYAML-cpp will be installed.")
    message(STATUS "\tURL: ${YAML_CPP_GIT_REPOSITORY}")
    message(STATUS "\tCOMMIT HASH: ${YAML_CPP_GIT_TAG}")

    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY ${YAML_CPP_GIT_REPOSITORY}
        GIT_TAG ${YAML_CPP_GIT_TAG}
    )
    FetchContent_MakeAvailable(yaml-cpp)
else()
    message(FATAL_ERROR "\tYAML-cpp not found.")
endif()
