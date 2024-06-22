---@diagnostic disable: undefined-global
workspace "glBlocks"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Application"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core"
    location "Core"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    postbuildcommands {
        ("{MKDIR} ../bin/" .. outputdir .. "/Application/"),
        ("{COPYFILE} %{cfg.buildtarget.relpath}/ ../bin/" .. outputdir .. "/Application/%{cfg.buildtarget.name}"),
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "%{wks.location}/dependencies/include",
        "%{prj.name}/src"
    }

    libdirs {
        "%{wks.location}/dependencies/lib"
    }

    pchheader "PCH.h"
    pchsource "%{prj.name}/src/PCH.cpp"
        
    filter "system:windows"
        links {
            "gdi32.lib",
            "opengl32.lib",
            "glfw3.lib"
        }

        defines {
            "BLOCKS_PLATFORM_WINDOWS",
            "BLOCKS_API_EXPORTS"
        }

    filter "system:linux"
        links {
            "GL",
            "glfw"
        }

        defines {
            "BLOCKS_PLATFORM_LINUX",
            "BLOCKS_API_EXPORTS"
        }

    filter "configurations:Debug"
        defines "BLOCKS_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "BLOCKS_RELEASE"
        runtime "Release"
        optimize "On"

    filter "files:Core/src/vendor/**.cpp"
        flags { "NoPCH" }
        
    filter "files:Core/src/vendor/**.c"
        flags { "NoPCH" }

project "Application"
    location "Application"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "%{wks.location}/dependencies/include",
        "%{wks.location}/Core/src"
    }

    links {
        "Core"
    }

    filter "system:windows"
        defines {
            "BLOCKS_PLATFORM_WINDOWS"
        }

        postbuildcommands {
            "if not exist \"%{wks.location}bin\\" ..outputdir.. "\\Application\\shaders\" mkdir \"%{wks.location}bin\\" ..outputdir.. "\\Application\\shaders\"",
            "if not exist \"%{wks.location}bin\\" ..outputdir.. "\\Application\\textures\" mkdir \"%{wks.location}bin\\" ..outputdir.. "\\Application\\textures\"",

            "xcopy \"%{prj.location}shaders\" \"%{wks.location}bin\\"  ..outputdir.. "\\Application\\shaders\" /E /Y /I /R",
            "xcopy \"%{prj.location}textures\" \"%{wks.location}bin\\"  ..outputdir.. "\\Application\\textures\" /E /Y /I /R",
        }

    filter "system:linux"
        defines {
            "BLOCKS_PLATFORM_LINUX"
        }

        postbuildcommands {
            "mkdir -p %{wks.location}/bin/" ..outputdir.. "/Application/shaders",
            "mkdir -p %{wks.location}/bin/" ..outputdir.. "/Application/textures",

            "cp -ru shaders/* %{wks.location}/bin/" ..outputdir.. "/Application/shaders",
            "cp -ru textures/* %{wks.location}/bin/" ..outputdir.. "/Application/textures",
        }

    filter "configurations:Debug"
        defines "BLOCKS_DEBUG"
        staticruntime "off"
        runtime "Debug"
        symbols "On"
        kind "ConsoleApp"
        entrypoint "mainCRTStartup"

    filter "configurations:Release"
        defines "BLOCKS_RELEASE"
        staticruntime "off"
        runtime "Release"
        optimize "On"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
