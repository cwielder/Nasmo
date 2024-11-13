#include <nsm/ui/uirenderer.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/gfx/primitiveshape.h>

#include <glm/gtc/matrix_transform.hpp>

nsm::UIRenderer::UIRenderer(glm::vec2* const position)
    : mPosition(position)
{
    if (sTextureShader == nullptr) {
        sTextureShader = new ShaderProgram("nsm/assets/shaders/ui_texture.vsh", "nsm/assets/shaders/ui_texture.fsh");
    }
}

void nsm::UIRenderer::drawTexture(const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation) const {
    sTextureShader->bind();

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, glm::vec3(*mPosition + translation, 0.0f));
    mtx = glm::rotate(mtx, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, glm::vec3(scale, 1.0f));

    sTextureShader->setOptionalMat4("uMtx", mtx);

    texture.bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
