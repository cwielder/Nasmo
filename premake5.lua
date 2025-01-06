-- premake5.lua
workspace "Nasmo"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

    filter "configurations:Debug"
        defines "TRACY_ENABLE"
    
    filter "configurations:Release"
        defines "TRACY_ENABLE"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Sandbox"
include "Nasmo"
