set(GHC_URL "https://github.com/gulrak/filesystem/archive/refs/tags/v1.5.10.zip")

message( STATUS "[GHC]" )
message( STATUS "\tGHC Will be installed." )
message( STATUS "\tURL: ${GHC_URL}" )

set(GHC_FILESYSTEM_WITH_INSTALL ON CACHE INTERNAL "Create import targets for ghc-filesystem")
FetchContent_Declare(
    GHC
    URL ${GHC_URL}
)
FetchContent_MakeAvailable(GHC)
