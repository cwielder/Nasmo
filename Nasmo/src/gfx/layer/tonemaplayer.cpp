#include <nsm/gfx/layer/tonemaplayer.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>

nsm::TonemapLayer::TonemapLayer(const std::string& name, const f32 exponent)
    : Layer(name)
    , mExponent(exponent)
    , mShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/tonemap_aces.fsh")
{
    mRenderState
        .blend(false)
        .cull(false)
        .depth(false)
    ;
}

void nsm::TonemapLayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    mShader.bind();
    mShader.setFloat("uExponent", mExponent);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
