#include <nsm/entity/component/skyboxcomponent.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/primitiveshape.h>

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
