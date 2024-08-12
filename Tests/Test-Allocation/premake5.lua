project "Test-Allocation"
    kind "ConsoleApp"
    language "C"
    cdialect "C17"

    debugdir(OUT_DIR)
    targetdir(OUT_DIR)
    objdir(INT_DIR)

    files
    {
        "./*.h",
        "./*.c"
    }

    includedirs
    {
        ".",
        "%{INTRICATE_ALLOCATOR_H_INCLUDE}"
    }

    defines
    {
        "IA_DYNAMIC"
    }

    links
    {
        "IntricateAllocator"
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"

    filter "system:macosx"
        systemversion "latest"
        cppdialect "C++latest"
