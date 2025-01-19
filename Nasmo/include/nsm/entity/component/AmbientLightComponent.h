#pragma once

#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/gfx/Shader.h>

#include <glm/vec3.hpp>

namespace nsm {

    class AmbientLightComponent : public DrawableComponent {
    public:
        AmbientLightComponent(const f32 intensity, const glm::vec3& color = glm::vec3(1.0f));
        ~AmbientLightComponent() override = default;

        void drawOpaque(const RenderInfo& renderInfo) override;
        void drawTranslucent(const RenderInfo& renderInfo) override { }

        [[nodiscard]] const glm::vec3& getColor() const { return mColor; }
        [[nodiscard]] f32 getIntensity() const { return mIntensity; }

        void setColor(const glm::vec3& color) { mColor = color; }
        void setIntensity(f32 intensity) { mIntensity = intensity; }

    private:
        ShaderProgram mShaderProgram;
        glm::vec3 mColor;
        f32 mIntensity;
    };

}
