# Macro for finding packages, or installing them from online repos using FetchContent
#
# Required args
# -------------
# pkg
#     Name of the package. This should be a simplified version of the full package name,
#     ideally in all-caps. It does not need to match the package name that is passed
#     to find_package()
#
# Optional args
# -------------
# PKG_NAME pkg_name
#     The 'true' package name, which will be passed to find_package(). If not set,
#     defaults to ${pkg}
# URL url
#     The URL from which to download the package if find_package() is not successful.
#     Usually set to a Github .zip archive.
# REPO repo
#     The URL of a Git repository to pull from. Ignored if URL is set. It is recommended
#     to always set TAG alongside this, or else the package will be pulled from the
#     latest commits to main/master, which may be unstable.
# TAG tag
#     The Git tag to use from REPO. Can be a commit hash, release tag, etc.
#     
# Environment vars
# ----------------
# FDPAPI_ALWAYS_FETCH_${pkg}
#     Set to ON to skip find_package() and always use FetchContent for this package.
# FDPAPI_NEVER_FETCH_${pkg}
#     Set to ON to skip FetchContent for this package. If find_package() is not
#     successful, the build will fail.

macro(fdpapi_add_external pkg)
    message(STATUS "[${pkg}]")
    cmake_parse_arguments(FDPAPI_${pkg} "" "PKG_NAME;URL;REPO;TAG" "" ${ARGN})

    # Set default PKG_NAME. This is passed to find_package
    if(NOT DEFINED FDPAPI_${pkg}_PKG_NAME)
        set(FDPAPI_${pkg}_PKG_NAME ${pkg})
    endif()

    # Set options to allow the user to control how they wish to use this package
    option(
        FDPAPI_NEVER_FETCH_${pkg}
        "Only use pre-installed ${pkg}, don't use FetchContent."
        OFF
    )
    option(
        FDPAPI_ALWAYS_FETCH_${pkg}
        "Don't use pre-installed ${pkg}, always use FetchContent."
        OFF
    )

    if(FDPAPI_ALWAYS_FETCH_${pkg} AND FDPAPI_NEVER_FETCH_${pkg})
        message(
            FATAL_ERROR
            "FDPAPI_ALWAYS_FETCH_${pkg} and FDPAPI_NEVER_FETCH_${pkg} are mutually exclusive"
        )
    endif()

    # Try to find package, unless the user has requested not to
    if(NOT (FDPAPI_ALWAYS_FETCH_${pkg} OR FDPAPI_ALWAYS_FETCH))
        find_package(${FDPAPI_${pkg}_PKG_NAME} QUIET)
    endif()

    # If found, write a message and continue. Otherwise, try FetchContent.
    if(${FDPAPI_${pkg}_PKG_NAME}_FOUND AND NOT (FDPAPI_ALWAYS_FETCH_${pkg} OR FDPAPI_ALWAYS_FETCH))
        message(STATUS "\t${pkg} found.")
    elseif(NOT (FDPAPI_NEVER_FETCH_${pkg} OR FDPAPI_NEVER_FETCH))
        message(STATUS "\t${pkg} not found.")

        # Prefer to use URL. Otherwise, use REPO, and prefer to use TAG in that case.
        if(DEFINED FDPAPI_${pkg}_URL)
            message(STATUS "\tInstalling from ${FDPAPI_${pkg}_URL}.")
            FetchContent_Declare(
                ${pkg}
                URL "${FDPAPI_${pkg}_URL}"
            )
        elseif(DEFINED FDPAPI_${pkg}_REPO)
            message(STATUS "\tInstalling from ${FDPAPI_${pkg}_REPO}.")
            if(DEFINED FDPAPI_${pkg}_TAG)
                message(STATUS "\tGit tag: ${FDPAPI_${pkg}_TAG}.")
                FetchContent_Declare(
                    ${pkg}
                    GIT_REPOSITORY "${FDPAPI_${pkg}_REPO}"
                    GIT_TAG "${FDPAPI_${pkg}_TAG}"
                )
            else()
                FetchContent_Declare(
                    ${pkg}
                    GIT_REPOSITORY "${FDPAPI_${pkg}_REPO}"
                )
            endif()
        else()
            message(FATAL_ERROR "\t${pkg} could not be installed.")
        endif()
        FetchContent_MakeAvailable(${pkg})
    else()
        message(FATAL_ERROR "\t${pkg} not found.")
    endif()
endmacro()
