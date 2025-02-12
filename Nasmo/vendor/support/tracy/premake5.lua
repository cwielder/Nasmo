project "tracy"
    kind "StaticLib"
    language "C++"
	cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    files {
        "../../tracy/public/TracyClient.cpp"
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
