project "msdf-atlas-gen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "off"
	vectorextensions "AVX2"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

	files {
		"../../msdf-atlas-gen/msdf-atlas-gen/**.h",
    	"../../msdf-atlas-gen/msdf-atlas-gen/**.hpp",
    	"../../msdf-atlas-gen/msdf-atlas-gen/**.cpp"
	}

	includedirs {
		"../../msdf-atlas-gen",
		"../../msdf-atlas-gen/msdfgen",
		"../../msdf-atlas-gen/msdfgen/include"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"msdfgen"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"

		project "freetype"
		location "freetype"
		kind "StaticLib"
		language "C"
		staticruntime "off"
	
		targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
		objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")
	
		files {
			"../../msdf-atlas-gen/msdfgen/freetype/include/ft2build.h",
			"../../msdf-atlas-gen/msdfgen/freetype/include/freetype/*.h",
			"../../msdf-atlas-gen/msdfgen/freetype/include/freetype/config/*.h",
			"../../msdf-atlas-gen/msdfgen/freetype/include/freetype/internal/*.h",
	
			"../../msdf-atlas-gen/msdfgen/freetype/src/autofit/autofit.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftbase.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftbbox.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftbdf.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftbitmap.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftcid.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftdebug.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftfstype.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftgasp.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftglyph.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftgxval.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftinit.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftmm.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftotval.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftpatent.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftpfr.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftstroke.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/svg/ftsvg.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftsynth.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftsystem.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/fttype1.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/base/ftwinfnt.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/bdf/bdf.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/bzip2/ftbzip2.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/cache/ftcache.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/cff/cff.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/cid/type1cid.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/gzip/ftgzip.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/lzw/ftlzw.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/pcf/pcf.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/pfr/pfr.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/psaux/psaux.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/pshinter/pshinter.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/psnames/psnames.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/raster/raster.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/sdf/sdf.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/sfnt/sfnt.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/smooth/smooth.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/truetype/truetype.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/type1/type1.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/type42/type42.c",
			"../../msdf-atlas-gen/msdfgen/freetype/src/winfonts/winfnt.c"
		}
	
		includedirs {
			"../../msdf-atlas-gen/msdfgen/freetype/include"
		}
	
		defines {
			"FT2_BUILD_LIBRARY",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS",
		}
	
		filter "system:windows"
			systemversion "latest"
	
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
	
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
	
		filter "configurations:Dist"
			runtime "Release"
			optimize "on"
			symbols "off"
	
project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")
	
	files {
		"../../msdf-atlas-gen/msdfgen/core/**.h",
		"../../msdf-atlas-gen/msdfgen/core/**.hpp",
		"../../msdf-atlas-gen/msdfgen/core/**.cpp",
		"../../msdf-atlas-gen/msdfgen/ext/**.h",
		"../../msdf-atlas-gen/msdfgen/ext/**.hpp",
		"../../msdf-atlas-gen/msdfgen/ext/**.cpp",
		"../../msdf-atlas-gen/msdfgen/lib/**.cpp",
		"../../msdf-atlas-gen/msdfgen/include/**.h"
	}
	
	includedirs {
		"../../msdf-atlas-gen/msdfgen/include",
		"../../msdf-atlas-gen/msdfgen/freetype/include"
	}
	
	defines {
		"MSDFGEN_USE_CPP11"
	}
	
	links {
		"freetype"
	}
	
	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"
