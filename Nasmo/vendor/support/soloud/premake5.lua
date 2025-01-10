project "soloud"
    kind "StaticLib"
    language "C++"
	cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    includedirs {
        "../../soloud/include"
    }

    files {
        "../../soloud/src/audiosource/**.cpp",
        "../../soloud/src/audiosource/**.c",
        "../../soloud/src/backend/miniaudio/soloud_miniaudio.cpp",
        "../../soloud/src/core/**.cpp",
        "../../soloud/src/filter/**.cpp",
    }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        optimize "off"
        symbols "on"
    
    filter "configurations:Release"
        defines "NDEBUG"
        runtime "Release"
        optimize "speed"
        symbols "on"
    
    filter "configurations:Dist"
        defines "NDEBUG"
        runtime "Release"
        optimize "speed"
        symbols "off"
