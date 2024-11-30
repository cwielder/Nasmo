#include <nsm/gfx/renderstate.h>

nsm::RenderState::RenderState()
    : mDepthTest(true)
    , mDepthWrite(true)
    , mCullEnabled(false)
    , mBlendEnabled(false)
    , mStencilTest(false)
    , mStencilMask(0xFF)
    , mStencilReference(0)
    , mStencilFunction(StencilFunction::Always)
    , mStencilFail(StencilOperation::Keep)
    , mDepthFail(StencilOperation::Keep)
    , mDepthPass(StencilOperation::Keep)
    , mDepthFunction(DepthFunction::Less)
    , mCullFace(CullFace::Back)
    , mCullDirection(CullDirection::CounterClockwise)
    , mSrcRGB(BlendFactor::SrcAlpha)
    , mSrcA(BlendFactor::SrcAlpha)
    , mDstRGB(BlendFactor::OneMinusSrcAlpha)
    , mDstA(BlendFactor::OneMinusSrcAlpha)
    , mBlendEquation(BlendEquation::Add)
{ }

void nsm::RenderState::apply(const nsm::RenderState::StateBit states) const {
    if (states & StateBit::Depth) {
        (mDepthTest ? glEnable : glDisable)(GL_DEPTH_TEST);
        if (mDepthTest) {
            glDepthFunc(static_cast<GLenum>(mDepthFunction));
            glDepthMask(mDepthWrite);
        }
    }

    if (states & StateBit::Stencil) {
        (mStencilTest ? glEnable : glDisable)(GL_STENCIL_TEST);
        if (mStencilTest) {
            glStencilMask(mStencilMask);
            glStencilFunc(static_cast<GLenum>(mStencilFunction), mStencilReference, mStencilMask);
            glStencilOp(static_cast<GLenum>(mStencilFail), static_cast<GLenum>(mDepthFail), static_cast<GLenum>(mDepthPass));
        }
    }

    if (states & StateBit::Cull) {
        (mCullEnabled ? glEnable : glDisable)(GL_CULL_FACE);
        if (mCullEnabled) {
            glCullFace(static_cast<GLenum>(mCullFace));
            glFrontFace(static_cast<GLenum>(mCullDirection));
        }
    }

    if (states & StateBit::Blend) {
        (mBlendEnabled ? glEnable : glDisable)(GL_BLEND);
        if (mBlendEnabled) {
            glBlendFuncSeparate(static_cast<GLenum>(mSrcRGB), static_cast<GLenum>(mDstRGB), static_cast<GLenum>(mSrcA), static_cast<GLenum>(mDstA));
            glBlendEquation(static_cast<GLenum>(mBlendEquation));
        }
    }
}
