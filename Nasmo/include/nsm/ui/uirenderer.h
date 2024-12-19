#pragma once

#include <nsm/common.h>
#include <nsm/gfx/shader.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/vertexbuffer.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Texture2D;
    struct RenderInfo;
    class UIPosition;
    class UIShape;
    class UIText;
    class Font;

    class UIRenderer final {
    public:
        enum class MaskMode {
            None,
            AsMask,
            Masked,
            InvertedMasked
        };

    public:
        UIRenderer(const UIPosition* const position);
        ~UIRenderer();

        void drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode = MaskMode::None) const;
        void drawPolygonSolid(const RenderInfo& renderInfo, const UIShape& shape, const glm::vec4& color, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode = MaskMode::None);
        void drawText(const RenderInfo& renderInfo, const UIText& text, const Font& font, const glm::vec4& color, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode = MaskMode::None) const;

    protected:
        void applyMaskMode(const RenderInfo& renderInfo, const MaskMode maskMode) const;

        const UIPosition* const mPosition;
        VertexBuffer* mVBO;
        VertexArray* mVAO;

        static inline ShaderProgram* sTextureShader = nullptr;
        static inline ShaderProgram* sPolygonSolidShader = nullptr;
        static inline ShaderProgram* sTextShader = nullptr;
    };

}
