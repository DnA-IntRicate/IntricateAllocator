include "Vendor/premake/customization/solutionitems.lua"

OUT_DIR = "%{wks.location}/bin/build/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/IntricateAllocator"
INT_DIR = "%{wks.location}/bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}"

-- This path is relative to the premake scripts for each Example/Test, not relative to this premake script
INTRICATE_ALLOCATOR_H_INCLUDE = "../../IntricateAllocator/src/include"

workspace "IntricateAllocator"
    architecture "x86_64"

    configurations
    {
        "Debug",
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

include "IntricateAllocator"
include "Tests"
