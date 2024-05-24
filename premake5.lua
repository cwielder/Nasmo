-- premake5.lua
workspace "Nasmo"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Sandbox"
include "Nasmo"
