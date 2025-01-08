#include <nsm/entity/component/SkyboxComponent.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/entity/component/CameraComponent.h>
#include <nsm/gfx/PrimitiveShape.h>

nsm::SkyboxComponent::SkyboxComponent(const std::string& path, const std::string& extension)
    : mShaderProgram("nsm/assets/shaders/skybox.vsh", "nsm/assets/shaders/skybox.fsh")
    , mCubemap(path, extension)
{ }

void nsm::SkyboxComponent::drawOpaque(const RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setMat4("uViewProjMtx", renderInfo.camera->getProjection() * glm::mat4(glm::mat3(renderInfo.camera->getView())));

    mCubemap.bind(0);

    nsm::PrimitiveShape::getCubeVAO().bind();
    nsm::PrimitiveShape::getCubeIBO().draw();
}
