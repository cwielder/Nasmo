project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")
    debugdir "../workdir"

    links {
        "Nasmo",

        -- TODO: Automate
        "GLFW",
        "glad",
        "simdjson",
        "fastgltf",
    }

    includedirs {
        "include",
        "../Nasmo/include",

        -- TODO: Automate
        "../Nasmo/vendor/glfw/include",
        "../Nasmo/vendor/glad/include",
        "../Nasmo/vendor/glm",
        "../Nasmo/vendor/simdjson/include",
        "../Nasmo/vendor/stb",
        "../Nasmo/vendor/imgui",
        "../Nasmo/vendor/fastgltf/include",
    }

    files {
        "src/**.cpp",
    }

    flags {
        "MultiProcessorCompile",
        "ShadowedVariables",
        "FatalWarnings"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "NSM_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
    
    filter "configurations:Debug"
        defines {
            "NSM_DEBUG",
            "_DEBUG"
        }
        runtime "Debug"
        optimize "off"
        symbols "on"
    
    filter "configurations:Release"
        defines "NSM_RELEASE"
        runtime "Release"
        optimize "speed"
        symbols "on"
        flags {
            "LinkTimeOptimization"
        }
    
    filter "configurations:Dist"
        defines {
            "NSM_DIST",
            "NSM_DISABLE_LOG_TRACE",
            "NSM_DISABLE_LOG_INFO"
        }
        runtime "Release"
        optimize "speed"
        symbols "off"
        flags {
            "LinkTimeOptimization"
        }
