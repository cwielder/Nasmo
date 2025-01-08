#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/VertexArray.h>
#include <nsm/gfx/VertexBuffer.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Texture2D;
    struct RenderInfo;
    class UIPosition;
    class UIShape;

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

        void drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation) const;
        void drawPolygonSolid(const RenderInfo& renderInfo, const UIShape& shape, const glm::vec4& color, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode = MaskMode::None);

    protected:
        const UIPosition* const mPosition;
        VertexBuffer* mVBO;
        VertexArray* mVAO;

        static inline ShaderProgram* sTextureShader = nullptr;
        static inline ShaderProgram* sPolygonSolidShader = nullptr;
    };

}
