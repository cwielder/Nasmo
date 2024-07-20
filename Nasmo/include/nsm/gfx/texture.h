#pragma once

#include <nsm/common.h>

#include <glm/vec2.hpp>

#include <nsm/gfx/opengl.h>

#include <unordered_map>
#include <string>

namespace nsm {

    class Texture final {
        NSM_NO_COPY(Texture);

    public:
        enum class FilterMode : u16 {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,

            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,

            Count
        };

        enum class WrapMode : u16 {
            Repeat = GL_REPEAT,
            MirroredRepeat = GL_MIRRORED_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
            ClampToBorder = GL_CLAMP_TO_BORDER,

            Count
        };

        enum class Format : u16 {
            R11G11B10F = GL_R11F_G11F_B10F,
            RGB16F = GL_RGB16F,
            RGB32F = GL_RGB32F,
            R8 = GL_R8,
            RGB8 = GL_RGB8,
            RGBA8 = GL_RGBA8,
            RGBA16F = GL_RGBA16F,
            RGBA32F = GL_RGBA32F,
            SRGB8 = GL_SRGB8,
            SRGBA8 = GL_SRGB8_ALPHA8,
            Depth24Stencil8 = GL_DEPTH24_STENCIL8,
            Depth32FStencil8 = GL_DEPTH32F_STENCIL8,
            
            Count
        };

    public:
        Texture();
        Texture(const std::string& path, bool srgb, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);
        Texture(const glm::u32vec2& size, const Format fmt, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);
        Texture(const u8* data, const std::size_t length, bool srgb = false, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);
        ~Texture();

        Texture(Texture&& other) noexcept
            : mId(other.mId)
            , mSize(other.mSize)
            , mFormat(other.mFormat)
            , mEnlargeFilter(other.mEnlargeFilter)
            , mShrinkFilter(other.mShrinkFilter)
            , mWrapS(other.mWrapS)
            , mWrapT(other.mWrapT)
        {
            other.mId = GL_NONE;
        }

        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {
                mId = other.mId;
                mSize = other.mSize;
                mFormat = other.mFormat;
                mEnlargeFilter = other.mEnlargeFilter;
                mShrinkFilter = other.mShrinkFilter;
                mWrapS = other.mWrapS;
                mWrapT = other.mWrapT;

                other.mId = GL_NONE;
            }

            return *this;
        }

        void initFromFile(const std::string& path, bool srgb = false, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);
        void initFromMemory(const u8* data, const std::size_t length, bool srgb = false, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);
        void initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, bool srgb = false, const FilterMode enlargeFilter = FilterMode::Linear, const FilterMode shrinkFilter = FilterMode::Linear, const WrapMode wrapS = WrapMode::Repeat, const WrapMode wrapT = WrapMode::Repeat);

        void bind(const u32 slot) const;

        [[nodiscard]] u32 getID() const { return mId; }
        [[nodiscard]] glm::vec2 getSize() const { return mSize; }
        [[nodiscard]] Format getFormat() const { return mFormat; }
        [[nodiscard]] FilterMode getEnlargeFilter() const { return mEnlargeFilter; }
        [[nodiscard]] FilterMode getShrinkFilter() const { return mShrinkFilter; }
        [[nodiscard]] WrapMode getWrapS() const { return mWrapS; }
        [[nodiscard]] WrapMode getWrapT() const { return mWrapT; }

        static void clearCache();

    private:
        static std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> sCache;

        glm::u32vec2 mSize;
        u32 mId;
        Format mFormat;
        FilterMode mEnlargeFilter, mShrinkFilter;
        WrapMode mWrapS, mWrapT;
    };

}
