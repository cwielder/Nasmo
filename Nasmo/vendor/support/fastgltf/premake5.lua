project "fastgltf"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"
    systemversion "latest"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    defines {
        "FASTGLTF_COMPILE_AS_CPP20=1"
    }

    includedirs {
        "../../fastgltf/include"
    }

    files {
        "../../fastgltf/src/fastgltf.cpp",
        "../../fastgltf/src/base64.cpp",
        "../../fastgltf/src/io.cpp"
    }

    -- SIMDJSON setup
    if os.isfile("../simdjson/premake5.lua") then
        includedirs "../../simdjson/include"
        links { "simdjson" }
    end

    flags {
        "MultiProcessorCompile"
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
