set(GHC_FILESYSTEM_WITH_INSTALL ON CACHE INTERNAL "Create import targets for ghc-filesystem")
fdpapi_add_external(
    "GHC"
    URL "https://github.com/gulrak/filesystem/archive/refs/tags/v1.5.10.zip"
    PKG_NAME "ghc_filesystem"
)
