#include <nsm/entity/component/AmbientLightComponent.h>

#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/gfx/Framebuffer.h>

nsm::AmbientLightComponent::AmbientLightComponent(const f32 intensity, const glm::vec3& color)
    : mColor(color)
    , mIntensity(intensity)
    , mShaderProgram("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/ambient_light.fsh")
{ }

void nsm::AmbientLightComponent::drawOpaque(const RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setOptionalVec3("uLightColor", mColor * mIntensity);

    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
