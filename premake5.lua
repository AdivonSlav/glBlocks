workspace "glBlocks"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Application"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core"
    location "Core"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "%{wks.location}/dependencies/include"
    }

    libdirs {
        "%{wks.location}/dependencies/lib"
    }

    links {
        "gdi32.lib",
        "opengl32.lib",
        "glfw3.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines {
            "BLOCKS_PLATFORM_WINDOWS",
            "BLOCKS_API_EXPORTS"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath}/ ../bin/" .. outputdir .. "/Application")
        }

    filter "configurations:Debug"
        defines "BLOCKS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BLOCKS_RELEASE"
        optimize "On"

project "Application"
    location "Application"
    language "C++"

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
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "BLOCKS_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "BLOCKS_DEBUG"
        symbols "On"
        kind "ConsoleApp"
        entrypoint "mainCRTStartup"

    filter "configurations:Release"
        defines "BLOCKS_RELEASE"
        optimize "On"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
