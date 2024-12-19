#include <nsm/ui/uirenderer.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/gfx/renderstate.h>
#include <nsm/gfx/font.h>
#include <nsm/ui/uiposition.h>
#include <nsm/ui/uishape.h>
#include <nsm/ui/uitext.h>
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

    if (sTextShader == nullptr) {
        sTextShader = new ShaderProgram("nsm/assets/shaders/ui_text.vsh", "nsm/assets/shaders/ui_text.fsh");
    }
}

nsm::UIRenderer::~UIRenderer() {
    delete mVBO;
    delete mVAO;
}

void nsm::UIRenderer::drawTexture(const RenderInfo& renderInfo, const Texture2D& texture, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode) const {
    this->applyMaskMode(renderInfo, maskMode);

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

    this->applyMaskMode(renderInfo, maskMode);

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

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
}

void nsm::UIRenderer::drawText(const RenderInfo& renderInfo, const UIText& text, const Font& font, const glm::vec4& color, const glm::vec2& scale, const f32 rotation, const glm::vec2& translation, const MaskMode maskMode) const {
    this->applyMaskMode(renderInfo, maskMode);

    font.getAtlas()->bind(0);

    sTextShader->bind();
    sTextShader->setMat4("uProjMtx", renderInfo.camera->getProjection());
    sTextShader->setOptionalVec4("uForegroundColor", color);
    sTextShader->setOptionalVec4("uBackgroundColor", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    PrimitiveShape::getQuadVAO().bind();

    const msdf_atlas::FontGeometry& fontGeometry = font.getGeometry();
    const msdfgen::FontMetrics& fontMetrics = fontGeometry.getMetrics();

    f64 fsScale = 1.0 / (fontMetrics.ascenderY - fontMetrics.descenderY);
    f64 x = 0.0;
    f64 y = 0.0;

    for (int i = 0; i < text.getText().size(); i++) {
        char character = text.getText()[i];
        char nextCharacter = i + 1 < text.getText().size() ? text.getText()[i + 1] : '\0';

        const msdf_atlas::GlyphGeometry* glyph = fontGeometry.getGlyph(character);
        if (glyph == nullptr) {
            glyph = fontGeometry.getGlyph('?');
        }

        NSM_ASSERT(glyph != nullptr, "Glyph not found in font atlas!");

        f64 atlasLeft, atlasBottom, atlasRight, atlasTop;
        glyph->getQuadAtlasBounds(atlasLeft, atlasBottom, atlasRight, atlasTop);
        glm::vec2 texCoordMin(atlasLeft, atlasBottom);
        glm::vec2 texCoordMax(atlasRight, atlasTop);

        f64 left, bottom, right, top;
        glyph->getQuadPlaneBounds(left, bottom, right, top);
        glm::vec2 quadMin(left, bottom);
        glm::vec2 quadMax(right, top);
        
        quadMin *= fsScale;
        quadMax *= fsScale;
        quadMin += glm::vec2(x, y);
        quadMax += glm::vec2(x, y);

        f32 texelWidth = 1.0f / font.getAtlas()->getSize().x;
        f32 texelHeight = 1.0f / font.getAtlas()->getSize().y;
        
        texCoordMin *= glm::vec2(texelWidth, texelHeight);
        texCoordMax *= glm::vec2(texelWidth, texelHeight);

        glm::vec2 center = (quadMin + quadMax) * 0.5f;
        glm::vec2 size = quadMax - quadMin;

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::scale(mtx, glm::vec3(-scale.x * size.x, scale.y * size.y, 1.0f));
        mtx = glm::rotate(mtx, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        mtx = glm::translate(mtx, glm::vec3(mPosition->getXY() + translation, 0.0f));
        mtx = glm::translate(mtx, glm::vec3(center, 0.0f));

        sTextShader->setMat4("uMtx", mtx);
        sTextShader->setVec4("uTexCoords", glm::vec4(texCoordMin, texCoordMax));

        PrimitiveShape::getQuadIBO().draw();

        if (nextCharacter != '\0') {
            f64 advance = glyph->getAdvance();
            fontGeometry.getAdvance(advance, character, nextCharacter);

            x += advance * fsScale;

            nsm::trace("Character: ", character, " Advance: ", advance, " x: ", x);
        }
    }
}

void nsm::UIRenderer::applyMaskMode(const RenderInfo& renderInfo, const MaskMode maskMode) const {
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
}
