#pragma once

#include <nsm/common.h>
#include <nsm/gfx/textureenum.h>

#include <glm/vec2.hpp>

#include <unordered_map>
#include <string>

namespace nsm {

    class Texture2D final {
        NSM_NO_COPY(Texture2D);

    public:
        Texture2D();
        Texture2D(const std::string& path, bool srgb, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);
        Texture2D(const glm::u32vec2& size, const Texture::Format fmt, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);
        Texture2D(const u8* data, const std::size_t length, bool srgb = false, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);
        ~Texture2D();

        Texture2D(Texture2D&& other) noexcept
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

        Texture2D& operator=(Texture2D&& other) noexcept {
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

        void initFromFile(const std::string& path, bool srgb = false, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);
        void initFromMemory(const u8* data, const std::size_t length, bool srgb = false, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);
        void initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, bool srgb = false, const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear, const Texture::WrapMode wrapS = Texture::WrapMode::Repeat, const Texture::WrapMode wrapT = Texture::WrapMode::Repeat);

        void bind(const u32 slot) const;

        [[nodiscard]] u32 getID() const { return mId; }
        [[nodiscard]] glm::vec2 getSize() const { return mSize; }
        [[nodiscard]] Texture::Format getFormat() const { return mFormat; }
        [[nodiscard]] Texture::FilterMode getEnlargeFilter() const { return mEnlargeFilter; }
        [[nodiscard]] Texture::FilterMode getShrinkFilter() const { return mShrinkFilter; }
        [[nodiscard]] Texture::WrapMode getWrapS() const { return mWrapS; }
        [[nodiscard]] Texture::WrapMode getWrapT() const { return mWrapT; }

        static void clearCache();

    private:
        static std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> sCache;

        glm::u32vec2 mSize;
        u32 mId;
        Texture::Format mFormat;
        Texture::FilterMode mEnlargeFilter, mShrinkFilter;
        Texture::WrapMode mWrapS, mWrapT;
    };

}
