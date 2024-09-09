#pragma once

#include <nsm/common.h>
#include <nsm/gfx/texture2D.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/fwd.hpp>

namespace nsm {

    class Framebuffer final {
        NSM_NO_COPY_OR_MOVE(Framebuffer);

    public:
        enum class BindMode {
            Draw,
            Read,
            Write,

            Count
        };

        enum class Type {
            Depth = 1 << 0,
            Color = 1 << 1,
            All = Depth | Color
        };

        friend u32 operator&(const nsm::Framebuffer::Type lhs, const nsm::Framebuffer::Type rhs) {
            return static_cast<u32>(lhs) & static_cast<u32>(rhs);
        }

    public:
        Framebuffer();
        ~Framebuffer();

        void bind(const BindMode mode = BindMode::Draw) const;
        void clear(const glm::f32vec4& value, const Type type, u32 drawBuffer = 0) const;
        void clear(const glm::u32vec4& value, const Type type, u32 drawBuffer = 0) const;
        void resize(const glm::u32vec2& size);

        [[nodiscard]] static Framebuffer* getBackbuffer();
        static void blit(const Framebuffer& src, const Framebuffer& dst, const glm::u32vec2& srcStart, const glm::u32vec2& srcEnd, const glm::u32vec2& dstStart, const glm::u32vec2& dstEnd, const u32 typeMask, const Texture2D::FilterMode filterMode = Texture2D::FilterMode::Linear);

        void addTextureBuffer(const Texture2D::Format fmt, const glm::u32vec2& size, const Texture2D::FilterMode enlargeFilter = Texture2D::FilterMode::Linear, const Texture2D::FilterMode shrinkFilter = Texture2D::FilterMode::Linear);
        //void addRenderBuffer(); // TODO: Implement
        void finalize() const;

        [[nodiscard]] const u32 getID() const { return mId; }
        [[nodiscard]] const Texture2D* getTextureBuffer(const std::size_t index) const { return mTextureBuffers[index]; }
        [[nodiscard]] const Texture2D* getDepthStencil() const { return mDepthStencil; }
        [[nodiscard]] const bool isFinalized() const { return mFinalized; }

    private:
        std::vector<Texture2D*> mTextureBuffers;
        u32 mId;
        Texture2D* mDepthStencil;
        bool mFinalized;
    };

}
