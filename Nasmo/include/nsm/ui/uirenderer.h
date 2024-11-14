#pragma once

#include <nsm/common.h>
#include <nsm/gfx/shader.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Texture2D;
    struct RenderInfo;

    class UIRenderer final {
    public:
        UIRenderer(glm::vec2* const position);
        ~UIRenderer() = default;

        void drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation) const;

    protected:
        glm::vec2* const mPosition;

        static inline ShaderProgram* sTextureShader = nullptr;
    };

}
