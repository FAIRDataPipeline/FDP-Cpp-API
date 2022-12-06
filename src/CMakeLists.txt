set(CPP_DATA_PIPELINE_SOURCE_FILES
    ../include/fdp/fdp.hxx
    ../include/fdp/exceptions.hxx
    ../include/fdp/objects/api_object.hxx
    ../include/fdp/objects/config.hxx
    ../include/fdp/objects/distribution.hxx
    ../include/fdp/objects/io_object.hxx
    ../include/fdp/objects/metadata.hxx
    ../include/fdp/registry/api.hxx
    ../include/fdp/utilities/data_io.hxx
    ../include/fdp/utilities/json.hxx
    ../include/fdp/utilities/logging.hxx
    ../include/fdp/utilities/semver.hxx
    ./fdp.cxx
    ./objects/api_object.cxx
    ./objects/config.cxx
    ./objects/distribution.cxx
    ./objects/metadata.cxx
    ./registry/api.cxx
    ./utilities/data_io.cxx
    ./utilities/json.cxx
    ./utilities/logging.cxx
    ./utilities/semver.cxx
)

if(WIN32)
    list(APPEND CPP_DATA_PIPELINE_SOURCE_FILES
        ../include/fdp/windows_sys/time.cpp 
        ../include/fdp/windows_sys/times.h 
        ../include/fdp/windows_sys/time.h 
    )
endif()

# Add the project library using SRC_FILES
add_library(cppDataPipeline SHARED ${CPP_DATA_PIPELINE_SOURCE_FILES})
add_library(cppDataPipeline::cppDataPipeline ALIAS cppDataPipeline)

# Set libraries compiled in debug mode to end in 'd'
set_target_properties(cppDataPipeline PROPERTIES DEBUG_POSTFIX "d")

# Set the public header
# FIXME This line breaks installation. How is it supposed to work?
#set_target_properties(cppDataPipeline PROPERTIES PUBLIC_HEADER "${CPP_DATA_PIPELINE_CODE_COVERAGE}")

target_compile_features(cppDataPipeline PUBLIC cxx_std_11)

target_include_directories(
    cppDataPipeline 
    PUBLIC
    $<BUILD_INTERFACE:${cppDataPipeline_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Dependencies
target_link_libraries(cppDataPipeline PRIVATE toml11::toml11)
target_link_libraries(cppDataPipeline PRIVATE jsoncpp_lib)
target_link_libraries(cppDataPipeline PRIVATE digestpp)  
target_link_libraries(cppDataPipeline PRIVATE boost_regex)
target_link_libraries(cppDataPipeline PRIVATE ${CURL_LIBRARIES})
target_link_libraries(cppDataPipeline PRIVATE yaml-cpp)

# ghc_filesystem is included at the top-level header file -- needs public linking
target_link_libraries(cppDataPipeline PUBLIC ghc_filesystem)

# Set rules for installing targets
set(cppDataPipeline_targets cppDataPipeline)
install(TARGETS ${cppDataPipeline_targets}
        EXPORT cppDataPipelineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export file, which contains code to allow other projects to import this project
install(
    EXPORT cppDataPipelineTargets
    FILE cppDataPipelineTargets.cmake
    NAMESPACE cppDataPipeline::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppDataPipeline
)

# Install this project's include dir, so projects importing it can access it
install(DIRECTORY ../include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create files that allow installed project to be discovered using find_package
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/cppDataPipelineConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/cppDataPipelineConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppDataPipeline
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cppDataPipelineConfigVersion.cmake"
    VERSION  ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Specify where to install the config files we generated
install(
    FILES 
        "${PROJECT_BINARY_DIR}/cppDataPipelineConfig.cmake"
        "${PROJECT_BINARY_DIR}/cppDataPipelineConfigVersion.cmake"
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/cppDataPipeline
)

# Configure the .in pkg-config file and install
CONFIGURE_FILE(
    ${PROJECT_SOURCE_DIR}/pkg-config/cppDataPipeline.pc.in cppDataPipeline.pc @ONLY
)

INSTALL(
    FILES 
        ${PROJECT_BINARY_DIR}/cppDataPipeline.pc
    DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

