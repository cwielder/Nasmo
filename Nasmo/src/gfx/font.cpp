#include <nsm/gfx/font.h>

#include <nsm/debug/assert.h>
#include <nsm/gfx/texture2D.h>

#include <msdf-atlas-gen.h>
#include <msdfgen.h>

#include <thread>

msdfgen::FreetypeHandle* nsm::Font::sFreeType = nullptr;

std::unordered_map<std::string, nsm::Font*> nsm::Font::sFontCache;

nsm::Font* nsm::Font::get(const std::string& path) {
    auto it = sFontCache.find(path);
    if (it != sFontCache.end()) {
        return it->second;
    }

    Font* font = new Font(path);
    sFontCache[path] = font;
    return font;
}

nsm::Font::Font(const std::string& path)
    : mGeometry(&mGlyphGeometry)
{
    if (sFreeType == nullptr) {
        sFreeType = msdfgen::initializeFreetype();
        NSM_ASSERT(sFreeType != nullptr, "Failed to initialize FreeType");
    }

    msdfgen::FontHandle* font = msdfgen::loadFont(sFreeType, path.c_str());
    NSM_ASSERT(font != nullptr, "Failed to load font ", path);

    msdf_atlas::Charset charset; // TODO: Cache this
    for (int i = 0x20; i <= 0xFF; i++) {
        charset.add(i);
    }

    constexpr double fontScale = 1.0;
    i32 numChars = mGeometry.loadCharset(font, fontScale, charset);

    nsm::trace("Loaded ", numChars, " glyphs from ", path);

    msdf_atlas::TightAtlasPacker packer;
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    packer.setPadding(0);
    packer.setScale(40.0);
    numChars = packer.pack(mGlyphGeometry.data(), mGlyphGeometry.size());
    NSM_ASSERT(numChars == 0, "Failed to pack all glyphs");

    i32 width, height;
    packer.getDimensions(width, height);

    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    msdf_atlas::ImmediateAtlasGenerator<f32, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<u8, 3>> generator(width, height);
    generator.setAttributes(attributes);
    generator.setThreadCount(std::thread::hardware_concurrency());
    generator.generate(mGlyphGeometry.data(), mGlyphGeometry.size());

    msdfgen::BitmapConstRef<u8, 3> atlas = generator.atlasStorage();

    mAtlas = new nsm::Texture2D(atlas.pixels, width * height * 3);

    msdfgen::destroyFont(font);
}

nsm::Font::~Font() {
    delete mAtlas;
}

void nsm::Font::destroy() {
    if (sFreeType != nullptr) {
        msdfgen::deinitializeFreetype(sFreeType);
        sFreeType = nullptr;
    }

    for (auto& [path, font] : sFontCache) {
        delete font;
    }

    sFontCache.clear();
}
