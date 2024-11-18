#pragma once

#include <nsm/common.h>
#include <nsm/gfx/shader.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Texture2D;
    struct RenderInfo;
    class UIPosition;

    class UIRenderer final {
    public:
        UIRenderer(const UIPosition* const position);
        ~UIRenderer() = default;

        void drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation) const;

    protected:
        const UIPosition* const mPosition;

        static inline ShaderProgram* sTextureShader = nullptr;
    };

}
