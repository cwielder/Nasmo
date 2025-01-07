#pragma once

#include <nsm/gfx/pipeline/renderpipeline.h>

#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/bloomlayer.h>
#include <nsm/gfx/layer/tonemaplayer.h>
#include <nsm/gfx/layer/imguilayer.h>
#include <nsm/gfx/layer/lightinglayer.h>
#include <nsm/gfx/layer/skyboxlayer.h>
#include <nsm/gfx/layer/forwardlayer.h>
#include <nsm/gfx/layer/uilayer.h>
#include <nsm/gfx/layer/fxaalayer.h>

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
        bool mDevMode = true; // TODO: Add a way to toggle this globally
    };

}
