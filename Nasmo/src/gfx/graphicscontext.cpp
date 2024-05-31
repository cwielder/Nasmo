#include <nsm/gfx/graphicscontext.h>

nsm::GraphicsContext::GraphicsContext()
    : mDepthTest(true)
    , mDepthWrite(true)
    , mDepthFunction(DepthFunction::Less)
    , mCullEnabled(false)
    , mCullFace(CullFace::Back)
    , mCullDirection(CullDirection::CounterClockwise)
    , mBlendEnabled(false)
    , mSrcRGB(BlendFactor::SrcAlpha)
    , mSrcA(BlendFactor::SrcAlpha)
    , mDstRGB(BlendFactor::OneMinusSrcAlpha)
    , mDstA(BlendFactor::OneMinusSrcAlpha)
    , mBlendEquation(BlendEquation::Add)
{ }

void nsm::GraphicsContext::apply() const {
    (mDepthTest ? glEnable : glDisable)(GL_DEPTH_TEST);
    if (mDepthTest) {
        glDepthFunc(static_cast<GLenum>(mDepthFunction));
        glDepthMask(mDepthWrite);
    }

    (mCullEnabled ? glEnable : glDisable)(GL_CULL_FACE);
    if (mCullEnabled) {
        glCullFace(static_cast<GLenum>(mCullFace));
        glFrontFace(static_cast<GLenum>(mCullDirection));
    }

    (mBlendEnabled ? glEnable : glDisable)(GL_BLEND);
    if (mBlendEnabled) {
        glBlendFuncSeparate(static_cast<GLenum>(mSrcRGB), static_cast<GLenum>(mDstRGB), static_cast<GLenum>(mSrcA), static_cast<GLenum>(mDstA));
        glBlendEquation(static_cast<GLenum>(mBlendEquation));
    }
}
