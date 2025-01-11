#pragma once

#include <nsm/gfx/pipeline/RenderPipeline.h>

#include <nsm/gfx/layer/ModelLayer.h>
#include <nsm/gfx/layer/BloomLayer.h>
#include <nsm/gfx/layer/TonemapLayer.h>
#include <nsm/gfx/layer/ImGuiLayer.h>
#include <nsm/gfx/layer/LightingLayer.h>
#include <nsm/gfx/layer/SkyboxLayer.h>
#include <nsm/gfx/layer/ForwardLayer.h>
#include <nsm/gfx/layer/UILayer.h>
#include <nsm/gfx/layer/FXAALayer.h>

namespace nsm {

    class StandardPipeline : public RenderPipeline {
    public:
        StandardPipeline();
        ~StandardPipeline() override = default;

        void render(nsm::Framebuffer* framebuffer) override;
        void onResize(const glm::u32vec2& size) override;

        nsm::SkyboxLayer* mLayerSkybox;
        nsm::ModelLayer* mLayerMain;
        nsm::LightingLayer* mLayerLightingDirectional;
        nsm::LightingLayer* mLayerLightingPoint;
        nsm::BloomLayer* mLayerBloom;
        nsm::TonemapLayer* mLayerTonemap;
        nsm::ForwardLayer* mLayerForward;
        nsm::FXAALayer* mLayerFXAA;
        nsm::UILayer* mLayerUI;
        nsm::ImGuiLayer* mLayerImGui;

        nsm::Framebuffer mGeometryBuffer;
        nsm::Framebuffer mLitObjectsBuffer;
        nsm::Framebuffer mGameBuffer;

        nsm::ShaderProgram mBlitShader;
        nsm::RenderState mBlitRenderState;

        bool mFirstFrame = true;
    };

}
