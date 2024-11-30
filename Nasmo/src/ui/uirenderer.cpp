#include <nsm/ui/uirenderer.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/gfx/renderstate.h>
#include <nsm/ui/uiposition.h>
#include <nsm/ui/uishape.h>
#include <nsm/entity/component/cameracomponent.h>

#include <glm/gtc/matrix_transform.hpp>

nsm::UIRenderer::UIRenderer(const UIPosition* const position)
    : mPosition(position)
    , mVBO(nullptr)
    , mVAO(nullptr)
{
    if (sTextureShader == nullptr) {
        sTextureShader = new ShaderProgram("nsm/assets/shaders/ui_texture.vsh", "nsm/assets/shaders/ui_texture.fsh");
    }
    if (sPolygonSolidShader == nullptr) {
        sPolygonSolidShader = new ShaderProgram("nsm/assets/shaders/ui_polygon_solid.vsh", "nsm/assets/shaders/ui_polygon_solid.fsh");
    }
}

nsm::UIRenderer::~UIRenderer() {
    delete mVBO;
    delete mVAO;
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

void nsm::UIRenderer::drawPolygonSolid(const RenderInfo& renderInfo, const UIShape& shape, const glm::vec4& color, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode) {
    if (mVBO == nullptr || mVAO == nullptr) [[unlikely]] {
        static constexpr f32 sNullVertices[(UIShape::cMaxVertices - 2) * 3 * 2] = { 0.0f };

        mVBO = new VertexBuffer(sNullVertices, sizeof(sNullVertices), 2 * sizeof(f32), BufferUsage::DynamicDraw);
        
        const std::array<VertexArray::Attribute, 1> attributes = {
            VertexArray::Attribute{0, 2, VertexArray::DataType::Float, 0, 0, false}
        };

        mVAO = new VertexArray();
        mVAO->setLayout(attributes);
        mVAO->linkBuffer(*mVBO, 0);
    }

    nsm::RenderState renderState;
    switch (maskMode) {
        case MaskMode::None:
            // Disable mask on renderstate
            renderState.stencil(false);
            break;
        case MaskMode::AsMask:
            renderInfo.framebuffer->clear(glm::i32vec4{ 0 }, Framebuffer::Type::Stencil, 1);
            // Enable mask write on renderstate
            renderState.stencil(RenderState::StencilFunction::Always, RenderState::StencilOperation::Replace, RenderState::StencilOperation::Replace, RenderState::StencilOperation::Replace, 0xFF, 0xFF);
            break;
        case MaskMode::Masked:
            // Enable mask read on renderstate
            renderState.stencil(RenderState::StencilFunction::Equal, RenderState::StencilOperation::Keep, RenderState::StencilOperation::Keep, RenderState::StencilOperation::Keep, 0xFF, 0xFF);
            break;
        case MaskMode::InvertedMasked:
            // Enable mask read on renderstate (inverted)
            renderState.stencil(RenderState::StencilFunction::NotEqual, RenderState::StencilOperation::Keep, RenderState::StencilOperation::Keep, RenderState::StencilOperation::Keep, 0xFF, 0xFF);
            break;
    }

    renderState.apply(RenderState::StateBit::Stencil);

    sPolygonSolidShader->bind();

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, glm::vec3(mPosition->getXY() + translation, 0.0f));
    mtx = glm::rotate(mtx, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, glm::vec3(-scale.x, scale.y, 1.0f));

    sPolygonSolidShader->setMat4(0, mtx);
    sPolygonSolidShader->setMat4(1, renderInfo.camera->getProjection());
    sPolygonSolidShader->setVec4(2, color);

    std::vector<glm::vec2> vertices = shape.getVerticesTriangulated();

    mVBO->subData(vertices.data(), vertices.size() * sizeof(glm::vec2), 0);

    mVAO->bind();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
