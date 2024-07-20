#include <nsm/gfx/layer/bloomlayer.h>
#include <nsm/gfx/graphics.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/gfx/viewport.h>

#include <nsm/debug/log.h>

#include <imgui.h>

nsm::BloomLayer::BloomLayer(const std::string& name)
    : Layer(name)
    , mFramebuffers()
    , mShaderDownsample("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/bloom_downsample.fsh")
    , mShaderUpsample("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/bloom_upsample.fsh")
    , mShaderBlit("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/blit.fsh")
{
    glm::u32vec2 size = Graphics::getFramebufferSize();
    for (u32 i = 0; i < cPasses; ++i) {
        mFramebuffers[i].addTextureBuffer(Texture::Format::R11G11B10F, size);
        size /= 2;
        mFramebuffers[i].finalize();
    }
}

void nsm::BloomLayer::draw(const nsm::RenderInfo& renderInfo) {
    Viewport viewport(*renderInfo.framebuffer->getTextureBuffer(0));
  
    this->downsample(renderInfo, viewport);
    this->upsample(renderInfo, viewport);
    this->blit(renderInfo, viewport);
}

void nsm::BloomLayer::downsample(const nsm::RenderInfo& renderInfo, nsm::Viewport& viewport) {
    mRenderState
        .blend(false)
        .cull(false)
        .depth(false)
        .apply()
    ;

    mShaderDownsample.bind();
    for (u32 i = 0; i < cPasses; ++i) {
        mFramebuffers[i].clear(glm::f32vec4(0.0f), Framebuffer::Type::Color);
        viewport.resize(mFramebuffers[i].getTextureBuffer(0)->getSize());
        viewport.apply();
  
        mShaderDownsample.setOptionalInt("uPass", i);

        if (i == 0) {
            renderInfo.framebuffer->getTextureBuffer(0)->bind(0);
            mShaderDownsample.setOptionalVec2("uInvResolution", glm::vec2(1.0) / glm::vec2(renderInfo.framebuffer->getTextureBuffer(0)->getSize()));
        } else {
            mFramebuffers[i - 1].getTextureBuffer(0)->bind(0);
            mShaderDownsample.setOptionalVec2("uInvResolution", glm::vec2(1.0) / glm::vec2(mFramebuffers[i - 1].getTextureBuffer(0)->getSize()));
        }

        mFramebuffers[i].bind();
        
        PrimitiveShape::getQuadVAO().bind();
        PrimitiveShape::getQuadIBO().bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void nsm::BloomLayer::upsample(const nsm::RenderInfo& renderInfo, nsm::Viewport& viewport) {
    mRenderState
        .blend(RenderState::BlendFactor::One, RenderState::BlendFactor::One, RenderState::BlendEquation::Add)
        .apply()    
    ;
    mShaderUpsample.bind();
    for (u32 i = cPasses - 1; i > 0; i--) {
        viewport.resize(mFramebuffers[i - 1].getTextureBuffer(0)->getSize());
        viewport.apply();

        mFramebuffers[i].getTextureBuffer(0)->bind(0);

        mFramebuffers[i - 1].bind();

        PrimitiveShape::getQuadVAO().bind();
        PrimitiveShape::getQuadIBO().bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void nsm::BloomLayer::blit(const nsm::RenderInfo& renderInfo, nsm::Viewport& viewport) {
    mFramebuffers[0].getTextureBuffer(0)->bind(0);

    mShaderBlit.bind();

    renderInfo.framebuffer->bind();

    viewport.resize(renderInfo.framebuffer->getTextureBuffer(0)->getSize());
    viewport.apply();

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void nsm::BloomLayer::resize(const glm::u32vec2& size) {
    glm::u32vec2 sizemt = Graphics::getFramebufferSize();
    for (u32 i = 0; i < cPasses; ++i) {
        mFramebuffers[i].resize(sizemt);
        sizemt /= 2;
    }
}
