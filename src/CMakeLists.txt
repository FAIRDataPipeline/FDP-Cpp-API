set(FDPAPI_SOURCE_FILES
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
    list(APPEND FDPAPI_SOURCE_FILES
        ../include/windows_sys/times.h 
        ../include/windows_sys/time.h
        ../include/windows_sys/time.cpp 
    )
endif()

# Add the project library using SRC_FILES
add_library(fdpapi ${FDPAPI_SOURCE_FILES})
add_library(fdpapi::fdpapi ALIAS fdpapi)

# Set libraries compiled in debug mode to end in 'd'
set_target_properties(fdpapi PROPERTIES DEBUG_POSTFIX "d")

target_compile_features(fdpapi PUBLIC cxx_std_11)

target_include_directories(
    fdpapi 
    PUBLIC
    $<BUILD_INTERFACE:${fdpapi_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Dependencies
target_link_libraries(fdpapi PRIVATE toml11::toml11)
target_link_libraries(fdpapi PRIVATE digestpp::digestpp)
target_link_libraries(fdpapi PRIVATE ${CURL_LIBRARIES})
target_link_libraries(fdpapi PRIVATE yaml-cpp)
target_link_libraries(fdpapi PRIVATE re2::re2)
target_link_libraries(fdpapi PRIVATE ghcFilesystem::ghc_filesystem)
if(BUILD_SHARED_LIBS)
    target_link_libraries(fdpapi PRIVATE jsoncpp_lib)
else()
    target_link_libraries(fdpapi PRIVATE jsoncpp_static)
endif()

# Set rules for installing targets
if(FDPAPI_WITH_INSTALL)
    message("Building With Install")
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib )
        set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin )
        set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include )
        message("installing includes to ${CMAKE_INSTALL_INCLUDEDIR}")
    endif()

    set(fdpapi_targets fdpapi)

    install(TARGETS ${fdpapi_targets}
            EXPORT fdpapiTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install export file, which contains code to allow other projects to import this project
    install(
        EXPORT fdpapiTargets
        FILE fdpapiTargets.cmake
        NAMESPACE fdpapi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapi
    )

    # Install this project's include dir, so projects importing it can access it
    install(DIRECTORY ../include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    # Create files that allow installed project to be discovered using find_package
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake_modules/fdpapiConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/fdpapiConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapi
    )

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/fdpapiConfigVersion.cmake"
        VERSION  ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion
    )

    # Specify where to install the config files we generated
    install(
        FILES 
            "${PROJECT_BINARY_DIR}/fdpapiConfig.cmake"
            "${PROJECT_BINARY_DIR}/fdpapiConfigVersion.cmake"
        DESTINATION 
            ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapi
    )

    # Configure the .in pkg-config file and install
    CONFIGURE_FILE(
        ${PROJECT_SOURCE_DIR}/pkg-config/fdpapi.pc.in ${PROJECT_BINARY_DIR}/fdpapi.pc @ONLY
    )

    INSTALL(
        FILES 
            ${PROJECT_BINARY_DIR}/fdpapi.pc
        DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
    )
endif()
