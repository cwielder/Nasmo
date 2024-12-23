project "Nasmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"
    systemversion "latest"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    defines "NSM_INTERNAL"

    links {
        "GLFW",
        "glad",
        "simdjson",
        "fastgltf",
        "poly2tri",
    }

    includedirs {
        "include",

        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/glm",
        "vendor/simdjson/include",
        "vendor/stb",
        "vendor/imgui",
        "vendor/fastgltf/include",
        "vendor/poly2tri",
    }

    libdirs {
        
    }

    files {
        "src/**.cpp"
    }

    flags {
        "MultiProcessorCompile",
        "FatalWarnings"
    }

    filter "system:windows"
        defines {
            "NSM_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
        links {
            
        }

    filter "system:linux"
        defines {
            "NSM_PLATFORM_LINUX"
        }
        links {
            
        }
    
    filter "configurations:Debug"
        defines {
            "NSM_DEBUG",
            "_DEBUG"
        }
        runtime "Debug"
        optimize "off"
        symbols "on"
        --sanitize "Address"
    
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

group "Dependencies"
    include "vendor/support/glfw"
    include "vendor/support/glad"
    include "vendor/support/simdjson"
    include "vendor/support/fastgltf"
    include "vendor/support/poly2tri"
group ""
