#include <nsm/gfx/texture2D.h>

#include <nsm/debug/assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> nsm::Texture2D::sCache;

nsm::Texture2D::Texture2D()
    : mId(GL_NONE)
    , mSize(0, 0)
    , mFormat()
    , mEnlargeFilter()
    , mShrinkFilter()
    , mWrapS()
    , mWrapT()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
}

nsm::Texture2D::Texture2D(const std::string& path, bool srgb, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT)
    : Texture2D()
{
    this->initFromFile(path, srgb, enlargeFilter, shrinkFilter, wrapS, wrapT);
}

nsm::Texture2D::Texture2D(const glm::u32vec2& size, const Texture::Format fmt, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT)
    : Texture2D()
{
    glTextureStorage2D(mId, 1, static_cast<GLenum>(fmt), size.x, size.y);

    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(enlargeFilter));
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(shrinkFilter));
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapS));
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapT));

    mSize = size;
    mFormat = fmt;
    mEnlargeFilter = enlargeFilter;
    mShrinkFilter = shrinkFilter;
    mWrapS = wrapS;
    mWrapT = wrapT;
}

nsm::Texture2D::Texture2D(const u8* data, const std::size_t length, bool srgb, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT)
    : Texture2D()
{
    this->initFromMemory(data, length, srgb, enlargeFilter, shrinkFilter, wrapS, wrapT);
}

nsm::Texture2D::~Texture2D() {
    if (mId != GL_NONE) {
        glDeleteTextures(1, &mId);
    }
}

void nsm::Texture2D::initFromFile(const std::string& path, bool srgb, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT) {
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

    this->initFromData(data, channels, size, srgb, enlargeFilter, shrinkFilter, wrapS, wrapT);

    nsm::trace("Loaded texture from file: ", path);
}

void nsm::Texture2D::initFromMemory(const u8* data, const std::size_t length, bool srgb, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT) {
    i32 width = 0, height = 0, channels = 0;
    u8* loadedData = stbi_load_from_memory(data, static_cast<int>(length), &width, &height, &channels, 0);
    nsm::trace("Loaded texture from memory: ", width, "x", height, " with ", channels, " channels");
    NSM_ASSERT(loadedData != nullptr, "Failed to load texture from memory");

    // TODO: Cache this data

    this->initFromData(loadedData, channels, { static_cast<u32>(width), static_cast<u32>(height) }, srgb, enlargeFilter, shrinkFilter, wrapS, wrapT);
}

void nsm::Texture2D::initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, bool srgb, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter, const Texture::WrapMode wrapS, const Texture::WrapMode wrapT) {
    NSM_ASSERT(mId != GL_NONE, "Cannot initialize invalid texture");

    mSize = size;

    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(enlargeFilter));
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(shrinkFilter));
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapS));
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapT));

    const auto safeSubImage = [this, data, size](const u32 format) {
        if (data == nullptr) [[unlikely]] {
            return;
        }

        glTextureSubImage2D(mId, 0, 0, 0, size.x, size.y, format, GL_UNSIGNED_BYTE, data);

        mFormat = static_cast<Texture::Format>(format);
    };

    if (srgb) {
        if (channelCount == 4) {
            glTextureStorage2D(mId, 1, GL_SRGB8_ALPHA8, size.x, size.y);
            safeSubImage(GL_RGBA);
        } else if (channelCount == 3) { 
            glTextureStorage2D(mId, 1, GL_SRGB8, size.x, size.y);
            safeSubImage(GL_RGB);
        } else if (channelCount == 1) {
            nsm::warn("Texture is grayscale, but is being loaded as sRGB. This may cause unexpected results.");
            glTextureStorage2D(mId, 1, GL_SRGB8, size.x, size.y);
            safeSubImage(GL_RED);
        } else {
            NSM_ASSERT(false, "Unknown texture channel count: ", channelCount);
        }
    } else {
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
    }

    glTextureParameteri(mId, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(mId, GL_TEXTURE_MAX_LEVEL, 6);
    glGenerateTextureMipmap(mId);

    mEnlargeFilter = enlargeFilter;
    mShrinkFilter = shrinkFilter;
    mWrapS = wrapS;
    mWrapT = wrapT;
}

void nsm::Texture2D::bind(const u32 slot) const {
    NSM_ASSERT(mId != GL_NONE, "Cannot bind invalid texture");

    glBindTextureUnit(slot, mId);
}

void nsm::Texture2D::clearCache() {
    for (const auto& [path, data] : sCache) {
        const auto& [dataPtr, size, channels] = data;

        stbi_image_free(dataPtr);
    }

    sCache.clear();
}
