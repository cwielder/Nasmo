#pragma once

#include <nsm/entity/component/DrawableComponent.h>

#include <nsm/gfx/Shader.h>
#include <nsm/gfx/VertexArray.h>
#include <nsm/gfx/VertexBuffer.h>
#include <nsm/gfx/IndexBuffer.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace nsm {

    class LightComponent : public DrawableComponent {
    protected:
        LightComponent(const glm::vec3& position, const glm::vec3& color, f32 intensity);
        ~LightComponent() override = default;

    public:
        void drawTranslucent(const RenderInfo& renderInfo) override { }

        [[nodiscard]] const glm::vec3& getPosition() const { return mPosition; }
        [[nodiscard]] const glm::vec3& getColor() const { return mColor; }
        [[nodiscard]] f32 getIntensity() const { return mIntensity; }

        void setPosition(const glm::vec3& position) { mPosition = position; mDirty = true; }
        void setColor(const glm::vec3& color) { mColor = color; }
        void setIntensity(f32 intensity) { mIntensity = intensity; }

    protected:
        glm::vec3 mPosition;
        glm::mat4 mModelMatrix;
        glm::vec3 mColor;
        f32 mIntensity;
        bool mDirty;
    };

    class PointLightComponent : public LightComponent {
    public:
        PointLightComponent(const glm::vec3& position, const glm::vec3& color, f32 intensity = 1.0f);
        ~PointLightComponent() override = default;

        void drawOpaque(const RenderInfo& renderInfo) override;

    private:
        ShaderProgram mShaderProgram;
    };

    class DirectionalLightComponent : public LightComponent {
    public:
        DirectionalLightComponent(const f32 yaw, const f32 pitch, const glm::vec3& color, f32 intensity = 1.0f);
        ~DirectionalLightComponent() override = default;

        void drawOpaque(const RenderInfo& renderInfo) override;

        [[nodiscard]] const glm::vec3& getDirection() const { return mDirection; }
        void setDirection(const f32 yaw, const f32 pitch) { mDirection = yawPitchToDirection(yaw, pitch); }

    private:
        static glm::vec3 yawPitchToDirection(const f32 yaw, const f32 pitch);

        ShaderProgram mShaderProgram;
        glm::vec3 mDirection;
        bool mDirty;
    };

}
