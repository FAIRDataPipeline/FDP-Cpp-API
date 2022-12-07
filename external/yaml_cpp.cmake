set(YAML_CPP_GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git")
set(YAML_CPP_GIT_TAG "1b50109f7bea60bd382d8ea7befce3d2bd67da5f")

message(STATUS "[YAML-cpp]")
message(STATUS "\tYAML-cpp Will be installed.")
message(STATUS "\tURL: ${YAML_CPP_GIT_REPOSITORY}")
message(STATUS "\tCOMMIT HASH: ${YAML_CPP_GIT_TAG}")

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "Build yaml-cpp tests")
if(BUILD_SHARED_LIBS)
    set(YAML_BUILD_SHARED_LIBS ON CACHE INTERNAL "Build .so for yaml-cpp")
endif()
set(YAML_CPP_INSTALL ON CACHE INTERNAL "Include export targets for installation")

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY ${YAML_CPP_GIT_REPOSITORY}
    GIT_TAG ${YAML_CPP_GIT_TAG}
)
FetchContent_MakeAvailable(yaml-cpp)
