#pragma once

#include <nsm/common.h>

#include <msdfgen.h>
#include <msdf-atlas-gen.h>

#include <string>
#include <unordered_map>

namespace nsm {

    class Texture2D;

    class Font {
    public:
        static Font* get(const std::string& path);
        static void destroy();

    private:
        Font(const std::string& path);
        ~Font();

    private:
        static msdfgen::FreetypeHandle* sFreeType;

        static std::unordered_map<std::string, Font*> sFontCache;

        msdf_atlas::FontGeometry mGeometry;
        std::vector<msdf_atlas::GlyphGeometry> mGlyphGeometry;
        Texture2D* mAtlas;
    };

}
