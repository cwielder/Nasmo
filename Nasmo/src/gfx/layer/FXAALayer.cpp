#include <nsm/gfx/layer/FXAALayer.h>
#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/gfx/Framebuffer.h>

nsm::FXAALayer::FXAALayer(const std::string& name)
    : Layer(name)
    , mShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/fxaa.fsh")
{
    mRenderState
        .blend(false)
        .cull(false)
        .depth(false)
    ;
}

void nsm::FXAALayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    glm::vec2 screenSize = renderInfo.framebuffer->getTextureBuffer(0)->getSize();

    mShader.bind();
    mShader.setVec2(0, { 1.0f / screenSize.x, 1.0f / screenSize.y });

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
