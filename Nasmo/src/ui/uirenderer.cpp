#include <nsm/ui/uirenderer.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/ui/uiposition.h>
#include <nsm/entity/component/cameracomponent.h>

#include <glm/gtc/matrix_transform.hpp>

nsm::UIRenderer::UIRenderer(const UIPosition* const position)
    : mPosition(position)
{
    if (sTextureShader == nullptr) {
        sTextureShader = new ShaderProgram("nsm/assets/shaders/ui_texture.vsh", "nsm/assets/shaders/ui_texture.fsh");
    }
}

void nsm::UIRenderer::drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation) const {
    sTextureShader->bind();

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, glm::vec3(mPosition->getXY() + translation, 0.0f));
    mtx = glm::rotate(mtx, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, glm::vec3(-scale.x, scale.y, 1.0f));

    sTextureShader->setMat4(0, mtx);
    sTextureShader->setMat4(1, renderInfo.camera->getProjection());

    texture.bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
