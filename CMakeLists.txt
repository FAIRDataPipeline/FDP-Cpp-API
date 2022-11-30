cmake_minimum_required(VERSION 3.13)

# Define Project
project(cppDataPipeline
    VERSION 0.2.6
    DESCRIPTION "C++ API for the FAIR Data Pipeline"
    HOMEPAGE_URL "https://github.com/FAIRDataPipeline/cppDataPipeline"
    LANGUAGES CXX C
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FetchContent)

# Default Options for Tests and Code Coverage
option(CPP_DATA_PIPELINE_BUILD_TESTS  "Build unit tests" OFF)
option(CPP_DATA_PIPELINE_CODE_COVERAGE "Run GCov and LCov code coverage tools" OFF)

# Set Module Path to include external directory
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/external")

# Include external library files
include(cmake/jsoncpp.cmake)
include(cmake/digestpp.cmake)
include(cmake/curl.cmake)
include(cmake/yaml_cpp.cmake)
include(cmake/toml11.cmake)
include(cmake/ghc.cmake)
include(cmake/boost_regex.cmake)

# Define and install library
add_subdirectory(./src)

# Compile Tests if specified
if(CPP_DATA_PIPELINE_BUILD_TESTS)
    # Include CTest for testing
    include(CTest)
    # Add the test dirctory compiling it's CMakeLists.txt
    add_subdirectory(test)
endif()

if(CPP_DATA_PIPELINE_CODE_COVERAGE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 --coverage")
    message("CXX Flags: ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
endif()

# Compile Code Coverage if Specified with Tests
if( CPP_DATA_PIPELINE_CODE_COVERAGE AND CPP_DATA_PIPELINE_BUILD_TESTS )
    if(CMAKE_COMPILER_IS_GNUCXX)
        LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
        include(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
        setup_target_for_coverage_gcovr_sonar(
            NAME coverage
            EXECUTABLE bin/cppDataPipeline-tests
            DEPENDENCIES cppDataPipeline-tests
            BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    endif()
endif()
