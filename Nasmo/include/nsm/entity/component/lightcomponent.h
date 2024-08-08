#pragma once

#include <nsm/entity/component/drawablecomponent.h>

#include <nsm/gfx/shader.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace nsm {

    class LightComponent : public DrawableComponent {
    protected:
        LightComponent(const glm::vec3& position, const glm::vec3& color, f32 intensity);
        ~LightComponent() override = default;

    public:
        void drawTranslucent(const RenderInfo& renderInfo) override { }

        virtual ShaderProgram* getShaderProgram() = 0;

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
        PointLightComponent(
            const glm::vec3& position,
            const glm::vec3& color,
            f32 intensity,
            f32 radius
        );

        ~PointLightComponent() override = default;

        void drawOpaque(const RenderInfo& renderInfo) override;

        ShaderProgram* getShaderProgram() override;

        [[nodiscard]] f32 getRadius() const { return mRadius; }
        void setRadius(f32 radius) { mRadius = radius; mDirty = true; }

    private:
        struct LightVolume {
            LightVolume() = default;

            VertexArray vao;
            VertexBuffer vbo;
            IndexBuffer ibo;
        };

        static LightVolume* sLightVolume;

        f32 mRadius;
        ShaderProgram mShaderProgram;
    };

}
