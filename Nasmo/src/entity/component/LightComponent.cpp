#include <nsm/entity/component/LightComponent.h>

#include <nsm/gfx/Shader.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/gfx/Framebuffer.h>
#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/entity/component/CameraComponent.h>

#include <glm/gtc/matrix_transform.hpp>

// LightComponent

nsm::LightComponent::LightComponent(const glm::vec3& position, const glm::vec3& color, const f32 intensity)
    : mPosition(position)
    , mColor(color)
    , mIntensity(intensity)
    , mDirty(true)
{ }

// PointLightComponent

nsm::PointLightComponent::PointLightComponent(const glm::vec3& position, const glm::vec3& color, const f32 intensity)
    : LightComponent(position, color, intensity)
    , mShaderProgram("nsm/assets/shaders/light_volume.vsh", "nsm/assets/shaders/point_light.fsh")
{ }

void nsm::PointLightComponent::drawOpaque(const RenderInfo& renderInfo) {    
    if (mDirty) {
        constexpr f32 cAttenuationLinear = 1.175f;
        constexpr f32 cAttenuationExp = 0.186f;

        const f32 maxChannel = glm::max(glm::max(mColor.r, mColor.g), mColor.b);

        // TODO: This calculation needs to be improved
        const f32 radius = (-cAttenuationLinear + std::sqrt(cAttenuationLinear * cAttenuationLinear - 4 * cAttenuationExp * (cAttenuationExp - 256 * mIntensity)));

        mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);
        mModelMatrix = glm::scale(mModelMatrix, glm::vec3(radius));
        mDirty = false;
    }

    mShaderProgram.bind();
    mShaderProgram.setOptionalMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mShaderProgram.setOptionalMat4("uModelMtx", mModelMatrix);
    mShaderProgram.setOptionalVec2("uScreenSize", renderInfo.framebuffer->getTextureBuffer(0)->getSize());
    mShaderProgram.setOptionalVec3("uCamPos", renderInfo.camera->getPosition());
    mShaderProgram.setOptionalVec3("uLightPos", mPosition);
    mShaderProgram.setOptionalVec3("uLightColor", mColor * mIntensity);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0); // normal+metallic
    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness
    renderInfo.framebuffer->getDepthStencil()->bind(2); // depth

    PrimitiveShape::getSphereVAO().bind();
    PrimitiveShape::getSphereIBO().draw();
}

// DirectionalLightComponent

nsm::DirectionalLightComponent::DirectionalLightComponent(const f32 yaw, const f32 pitch, const glm::vec3& color, const f32 intensity)
    : LightComponent(glm::vec3(0.0f), color, intensity)
    , mShaderProgram("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/directional_light.fsh")
    , mDirection(yawPitchToDirection(yaw, pitch))
{ }

void nsm::DirectionalLightComponent::drawOpaque(const RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setOptionalMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mShaderProgram.setOptionalVec3("uCamPos", renderInfo.camera->getPosition());
    mShaderProgram.setOptionalVec3("uLightDir", mDirection);
    mShaderProgram.setOptionalVec3("uLightColor", mColor * mIntensity);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0); // normal+metallic
    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness
    renderInfo.framebuffer->getDepthStencil()->bind(2); // depth

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}

glm::vec3 nsm::DirectionalLightComponent::yawPitchToDirection(const f32 yaw, const f32 pitch) {
    const f32 yawCos = std::cos(glm::radians(yaw));
    const f32 yawSin = std::sin(glm::radians(yaw));
    const f32 pitchCos = std::cos(glm::radians(pitch));
    const f32 pitchSin = std::sin(glm::radians(pitch));

    return glm::vec3(
        yawCos * pitchCos,
        yawSin * pitchCos,
        pitchSin
    );
}
