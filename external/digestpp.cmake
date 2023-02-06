fdpapi_add_external(
    "DIGESTPP"
    PKG_NAME "digestpp"
    REPO "https://github.com/LiamPattinson/digestpp.git"
    TAG "6f6f134"
)

# windows.h will conflict with min functions in digestcpp
# Because of macro definitions of min and max
# So tell the compiler to exclude min and max macros in windows
if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-DNOGDI)
endif()
