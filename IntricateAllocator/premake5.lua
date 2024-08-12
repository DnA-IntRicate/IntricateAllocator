project "IntricateAllocator"
    kind "SharedLib"
    language "C"
    cdialect "C17"

    debugdir(OUT_DIR)
    targetdir(OUT_DIR)
    objdir(INT_DIR)

    files
    {
        "./**.h",
        "./**.c"
    }

    includedirs
    {
        ".",
        "src/include/IntricateAllocator"
    }

    defines
    {
        "IA_DYNAMIC",
        "IA_BUILD"
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"

    filter "system:macosx"
        systemversion "latest"
        cppdialect "C++latest"
