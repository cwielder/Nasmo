#include <nsm/gfx/texture.h>

#include <nsm/debug/assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> nsm::Texture::sCache;

nsm::Texture::Texture()
    : mId(GL_NONE)
    , mSize(0, 0)
    , mFormat(Format::Count)
    , mFilterMode(FilterMode::Count)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
}

nsm::Texture::Texture(const std::string& path, const FilterMode filterMode)
    : Texture()
{
    this->initFromFile(path, filterMode);
}

nsm::Texture::Texture(const glm::u32vec2& size, const Format fmt, const FilterMode filterMode)
    : Texture()
{
    glTextureStorage2D(mId, 1, static_cast<GLenum>(fmt), size.x, size.y);

    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filterMode));
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filterMode));

    mSize = size;
    mFormat = fmt;
    mFilterMode = filterMode;
}

nsm::Texture::~Texture() {
    if (mId != GL_NONE) [[likely]] {
        glDeleteTextures(1, &mId);
    }
}

void nsm::Texture::initFromFile(const std::string& path, const FilterMode filterMode) {
    NSM_ASSERT(mId != GL_NONE, "Cannot initialize invalid texture");

    const auto load = [&path]() -> std::tuple<u8*, glm::u32vec2, u32> {
        for (const auto& [cachedPath, cachedData] : sCache) {
            if (cachedPath == path) {
                return cachedData;
            }
        }

        i32 width = 0, height = 0, channels = 0;

        stbi_set_flip_vertically_on_load(true);
        u8* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        NSM_ASSERT(data != nullptr, "Failed to load texture from file: ", path);

        sCache.emplace(path, std::make_tuple(data, glm::u32vec2(width, height), static_cast<u32>(channels)));

        return { data, { static_cast<u32>(width), static_cast<u32>(height) }, static_cast<u32>(channels) };
    };

    const auto& [data, size, channels] = load();

    this->initFromData(data, channels, size, filterMode);

    nsm::trace("Loaded texture from file: ", path);
}

void nsm::Texture::initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, const FilterMode filterMode) {
    NSM_ASSERT(mId != GL_NONE, "Cannot initialize invalid texture");

    mSize = size;

    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filterMode));
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filterMode));

    const auto safeSubImage = [this, data, size](const u32 format) {
        if (data == nullptr) [[unlikely]] {
            return;
        }

        glTextureSubImage2D(mId, 0, 0, 0, size.x, size.y, format, GL_UNSIGNED_BYTE, data);

        mFormat = static_cast<Format>(format);
    };

    if (channelCount == 4) {
        glTextureStorage2D(mId, 1, GL_RGBA8, size.x, size.y);
        safeSubImage(GL_RGBA);
    } else if (channelCount == 3) {
        glTextureStorage2D(mId, 1, GL_RGB8, size.x, size.y);
        safeSubImage(GL_RGB);
    } else if (channelCount == 1) {
        glTextureStorage2D(mId, 1, GL_R8, size.x, size.y);
        safeSubImage(GL_RED);
    } else [[unlikely]] {
        NSM_ASSERT(false, "Unknown texture channel count: ", channelCount);
    }

    glTextureParameteri(mId, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(mId, GL_TEXTURE_MAX_LEVEL, 6);
    glGenerateTextureMipmap(mId);

    mFilterMode = filterMode;
}

void nsm::Texture::bind(const u32 slot) const {
    NSM_ASSERT(mId != GL_NONE, "Cannot bind invalid texture");

    glBindTextureUnit(slot, mId);
}

void nsm::Texture::clearCache() {
    for (const auto& [path, data] : sCache) {
        const auto& [dataPtr, size, channels] = data;

        stbi_image_free(dataPtr);
    }

    sCache.clear();
}
