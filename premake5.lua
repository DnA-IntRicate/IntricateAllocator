include "Vendor/premake/customization/solutionitems.lua"

OUT_DIR = "%{wks.location}/bin/build/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/IntricateAllocator"
INT_DIR = "%{wks.location}/bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}"

workspace "IntricateAllocator"
    architecture "x86_64"
    startproject "intricate_allocator"

    configurations
    {
        "Debug",
        "Dev",
        "Release"
    }

    solutionitems
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    defines
    {
        "_CRT_SECURE_NO_DEPRECATE",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "Off"

        defines
        {
            "IA_PLATFORM_WINDOWS"
        }

    filter "system:linux"
        systemversion "latest"
        pic "On"
        staticruntime "Off"

        defines
        {
            "IA_PLATFORM_LINUX"
        }

    filter "system:macosx"
        systemversion "latest"
        pic "On"
        staticruntime "Off"

        defines
        {
            "IA_PLATFORM_OSX"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "Full"

        defines
        {
            "IA_DEBUG",
            "_DEBUG"
        }

    filter "configurations:Dev"
        runtime "Release"
        symbols "On"
        optimize "On"

        defines
        {
            "IA_DEV",
            "NDEBUG",
        }

    filter "configurations:Release"
        runtime "Release"
        symbols "Off"
        optimize "Full"

        defines
        {
            "IA_RELEASE",
            "NDEBUG"
        }

        flags
        {
            "NoBufferSecurityCheck",
            "NoRuntimeChecks",
            "LinkTimeOptimization",
            "NoIncrementalLink"
        }

-- PROJECT: intricate_allocator
project "intricate_allocator"
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
        "."
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"

    filter "system:macosx"
        systemversion "latest"
        cppdialect "C++latest"
