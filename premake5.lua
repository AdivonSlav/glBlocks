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
        cppdialect "C++20"
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
        staticruntime "off"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "BLOCKS_RELEASE"
        staticruntime "off"
        runtime "Release"
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
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "BLOCKS_PLATFORM_WINDOWS"
        }

        postbuildcommands {
            "if not exist \"%{wks.location}bin\\" ..outputdir.. "\\Application\\shaders\" mkdir \"%{wks.location}bin\\" ..outputdir.. "\\Application\\shaders\"",
            "if not exist \"%{wks.location}bin\\" ..outputdir.. "\\Application\\textures\" mkdir \"%{wks.location}bin\\" ..outputdir.. "\\Application\\textures\"",
            "if not exist \"%{wks.location}bin\\" ..outputdir.. "\\Application\\chunks\" mkdir \"%{wks.location}bin\\" ..outputdir.. "\\Application\\chunks\"",

            "xcopy \"%{prj.location}src\\shaders\" \"%{wks.location}bin\\"  ..outputdir.. "\\Application\\shaders\" /E /Y /I /R",
            "xcopy \"%{prj.location}src\\textures\" \"%{wks.location}bin\\"  ..outputdir.. "\\Application\\textures\" /E /Y /I /R",
            "xcopy \"%{prj.location}src\\chunks\" \"%{wks.location}bin\\"  ..outputdir.. "\\Application\\chunks\" /E /Y /I /R"
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
