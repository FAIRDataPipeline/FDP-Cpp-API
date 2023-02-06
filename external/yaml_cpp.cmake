set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "Build yaml-cpp tests")
if(BUILD_SHARED_LIBS)
    set(YAML_BUILD_SHARED_LIBS ON CACHE INTERNAL "Build .so for yaml-cpp")
endif()
set(YAML_CPP_INSTALL ON CACHE INTERNAL "Include export targets for installation")

fdpapi_add_external(
    "YAMLCPP"
    REPO "https://github.com/jbeder/yaml-cpp.git"
    TAG "1b50109f7bea60bd382d8ea7befce3d2bd67da5f"
    PKG_NAME "yaml-cpp"
)
