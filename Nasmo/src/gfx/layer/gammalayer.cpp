#include <nsm/gfx/layer/gammalayer.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>

nsm::GammaLayer::GammaLayer(const std::string& name, const f32 exponent)
    : Layer(name)
    , mExponent(exponent)
    , mShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/gamma.fsh")
    , mRenderState()
{
    mRenderState
        .blend(false)
        .cull(false)
        .depth(false)
    ;
}

void nsm::GammaLayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    mShader.bind();
    mShader.setFloat("uExponent", mExponent);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
