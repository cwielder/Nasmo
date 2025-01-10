#include <nsm/gfx/pipeline/StandardPipeline.h>

#include <nsm/gfx/Graphics.h>
#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/gfx/RenderInfo.h>

#include <imgui.h>

nsm::StandardPipeline::StandardPipeline()
    : mBlitShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/blit_threshold.fsh")
{
    mLayerSkybox = this->pushLayer<nsm::SkyboxLayer>("skybox");
    mLayerMain = this->pushLayer<nsm::ModelLayer>("main");
    mLayerLightingDirectional = this->pushLayer<nsm::LightingLayer>("lighting_directional", nsm::LightingLayer::Type::Directional);
    mLayerLightingPoint = this->pushLayer<nsm::LightingLayer>("lighting_point", nsm::LightingLayer::Type::Point);
    mLayerForward = this->pushLayer<nsm::ForwardLayer>("forward");
    mLayerFXAA = this->pushLayer<nsm::FXAALayer>("fxaa");
    mLayerBloom = this->pushLayer<nsm::BloomLayer>("bloom");
    mLayerTonemap = this->pushLayer<nsm::TonemapLayer>("cc");
    mLayerUI = this->pushLayer<nsm::UILayer>("ui");
    mLayerImGui = this->pushLayer<nsm::ImGuiLayer>("imgui");

    const glm::u32vec2 size = nsm::Graphics::getFramebufferSize();

    mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // normal+metallic
    mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // albedo+roughness
    mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::Depth32FStencil8, size);
    mGeometryBuffer.finalize();

    mLitObjectsBuffer.addTextureBuffer(nsm::Texture::Format::RGB16F, size);
    mLitObjectsBuffer.finalize();

    mGameBuffer.addTextureBuffer(nsm::Texture::Format::RGB16F, size); // color
    mGameBuffer.addTextureBuffer(nsm::Texture::Format::RGB16F, size); // emissive
    mGameBuffer.addTextureBuffer(nsm::Texture::Format::Depth32FStencil8, size);
    mGameBuffer.finalize();

    mBlitRenderState
        .blend(false)
        .stencil(nsm::RenderState::StencilFunction::Equal, nsm::RenderState::StencilOperation::Keep, nsm::RenderState::StencilOperation::Keep, nsm::RenderState::StencilOperation::Keep, 0xFF, 0xFF)
        .depth(false)
    ;

    if (mDevMode) {
        ImGui::DockSpaceOverViewport();
    }
}

void nsm::StandardPipeline::render(nsm::Framebuffer *framebuffer) {
    mGameBuffer.bind();
    mGameBuffer.clear(glm::f32vec4{0.0f}, nsm::Framebuffer::Type::Color, 0);
    mGameBuffer.clear(glm::f32vec4{0.0f}, nsm::Framebuffer::Type::Color, 1);
    mGameBuffer.clear(glm::f32vec4{1.0f}, nsm::Framebuffer::Type::Depth);

    // Skybox pass
    mLayerSkybox->draw({ mLayerMain->getCamera(), &mGameBuffer });

    // Build geometry buffer
    mGeometryBuffer.bind();

    if (!mFirstFrame) {
        mGeometryBuffer.takeDepthStencil(mGameBuffer);
    }
    
    mFirstFrame = false;
    mGeometryBuffer.clear(glm::f32vec4{0.0f}, nsm::Framebuffer::Type::Color, 0);
    mGeometryBuffer.clear(glm::f32vec4{0.0f}, nsm::Framebuffer::Type::Color, 1);
    mGeometryBuffer.clear(glm::f32vec4{1.0f}, nsm::Framebuffer::Type::Depth, 2);
    mGeometryBuffer.clear(glm::i32vec4{0}, nsm::Framebuffer::Type::Stencil, 2);
    mLayerMain->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

    // Lighting pass
    mLitObjectsBuffer.bind();
    mLitObjectsBuffer.clear(glm::f32vec4{0.0f}, nsm::Framebuffer::Type::Color, 0);
    mLayerLightingDirectional->draw({ mLayerMain->getCamera(), &mGeometryBuffer });
    mLayerLightingPoint->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

    // Blit lit objects to main framebuffer with stencil mask
    mGameBuffer.bind();
    mGameBuffer.takeDepthStencil(mGeometryBuffer);
    mBlitShader.bind();
    mBlitRenderState.apply();
    mLitObjectsBuffer.getTextureBuffer(0)->bind(0);
    nsm::PrimitiveShape::getQuadVAO().bind();
    nsm::PrimitiveShape::getQuadIBO().draw();

    mLayerForward->draw({ mLayerForward->getCamera(), &mGameBuffer });

    // Post-process pass
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT);
    mLayerFXAA->draw({ nullptr, &mGameBuffer });
    mLayerBloom->draw({ nullptr, &mGameBuffer });
    mLayerTonemap->draw({ nullptr, &mGameBuffer });

    // UI pass
    mGameBuffer.clear(glm::f32vec4{1.0f}, nsm::Framebuffer::Type::Depth);
    mLayerUI->draw({ nullptr, &mGameBuffer });

    framebuffer->bind();

    if (mDevMode) {
        // ImGui pass
        // TODO: Scale render viewport to fit pane

        ImGui::Begin("Viewport");
        const nsm::Texture2D *gameBufferTexture = mGameBuffer.getTextureBuffer(0);

        ImGui::Image(gameBufferTexture->getID(), ImVec2(gameBufferTexture->getSize().x, gameBufferTexture->getSize().y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        mLayerImGui->draw({nullptr, framebuffer});

        ImGui::DockSpaceOverViewport();
    } else {
        Framebuffer::blit(mGameBuffer, *framebuffer, { 0, 0 }, { framebuffer->getTextureBuffer(0)->getSize() }, { 0, 0 }, { framebuffer->getTextureBuffer(0)->getSize() }, (u32)Framebuffer::Type::Color);

        mLayerImGui->draw({nullptr, framebuffer});
    }
}

void nsm::StandardPipeline::onResize(const glm::u32vec2 &size) {
    mGeometryBuffer.resize(size);
    mLitObjectsBuffer.resize(size);
    mGameBuffer.resize(size);
}
