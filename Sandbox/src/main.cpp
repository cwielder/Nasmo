#include <nsm/app/application.h>
#include <nsm/gfx/renderer.h>
#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/bloomlayer.h>
#include <nsm/gfx/layer/tonemaplayer.h>
#include <nsm/gfx/layer/imguilayer.h>
#include <nsm/gfx/layer/lightinglayer.h>
#include <nsm/gfx/layer/skyboxlayer.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/gfx/renderpipeline.h>
#include <nsm/gfx/primitiveshape.h>

#include <unordered_map>

#include <imgui.h>

class DebugLayer : public nsm::Layer {
public:
    DebugLayer(const std::string& name)
        : Layer(name)
    {
        mRenderState
            .depth(nsm::RenderState::DepthFunction::Less, true)
        ;
    }

    void draw(const nsm::RenderInfo& renderInfo) {
        mRenderState.apply();

        for (auto& drawable : mDrawables) {
            drawable->drawOpaque(renderInfo);
        }
    }
};

class DemoPipeline : public nsm::RenderPipeline {
public:
    DemoPipeline()
        : mBlitShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/blit.fsh")
    {
        mLayerSkybox = this->pushLayer<nsm::SkyboxLayer>("skybox");
        mLayerMain = this->pushLayer<nsm::ModelLayer>("main");
        mLayerLightingDirectional = this->pushLayer<nsm::LightingLayer>("lighting_directional", nsm::LightingLayer::Type::Directional);
        mLayerLightingPoint = this->pushLayer<nsm::LightingLayer>("lighting_point", nsm::LightingLayer::Type::Point);
        mLayerBloom = this->pushLayer<nsm::BloomLayer>("bloom");
        mLayerTonemap = this->pushLayer<nsm::TonemapLayer>("cc");
        mLayerDebug = this->pushLayer<DebugLayer>("debug");
        mLayerImGui = this->pushLayer<nsm::ImGuiLayer>("imgui");

        glm::u32vec2 size = nsm::Graphics::getFramebufferSize();
        
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // normal+metallic
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // albedo+roughness
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::Depth32FStencil8, size);
        mGeometryBuffer.finalize();

        mLitObjectsBuffer.addTextureBuffer(nsm::Texture::Format::RGB16F, size);
        mLitObjectsBuffer.finalize();

        mBlitRenderState
            .blend(false)
            .stencil(nsm::RenderState::StencilFunction::Equal, nsm::RenderState::StencilOperation::Keep, nsm::RenderState::StencilOperation::Keep, nsm::RenderState::StencilOperation::Keep, 0xFF, 0xFF)
            .depth(false)
        ;
    }

    void render(nsm::Framebuffer* framebuffer) override {
        // Skybox pass
        mLayerSkybox->draw({ mLayerMain->getCamera(), framebuffer });

        // Build geometry buffer
        mGeometryBuffer.bind();
        if (!mFirstFrame) {
            mGeometryBuffer.takeDepthStencil(*framebuffer);
        }
        mFirstFrame = false;
        mGeometryBuffer.clear(glm::f32vec4{ 0.0f }, nsm::Framebuffer::Type::Color, 0);
        mGeometryBuffer.clear(glm::f32vec4{ 0.0f }, nsm::Framebuffer::Type::Color, 1);
        mGeometryBuffer.clear(glm::f32vec4{ 1.0f }, nsm::Framebuffer::Type::Depth, 2);
        mGeometryBuffer.clear(glm::i32vec4{ 0 }, nsm::Framebuffer::Type::Stencil, 2);
        mLayerMain->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

        // Lighting pass
        mLitObjectsBuffer.bind();
        mLitObjectsBuffer.clear(glm::f32vec4{ 0.0f }, nsm::Framebuffer::Type::Color, 0);
        mLayerLightingDirectional->draw({ mLayerMain->getCamera(), &mGeometryBuffer });
        mLayerLightingPoint->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

        // Blit lit objects to main framebuffer with stencil mask
        framebuffer->bind();
        framebuffer->takeDepthStencil(mGeometryBuffer);
        mBlitShader.bind();
        mBlitRenderState.apply();
        mLitObjectsBuffer.getTextureBuffer(0)->bind(0);
        nsm::PrimitiveShape::getQuadVAO().bind();
        nsm::PrimitiveShape::getQuadIBO().draw();

        // Post-process pass
        mLayerBloom->draw({ nullptr, framebuffer });
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        mLayerTonemap->draw({ nullptr, framebuffer });

        // Debug pass
        mLayerDebug->draw({ mLayerDebug->getCamera(), framebuffer });

        // ImGui pass
        mLayerImGui->draw({ nullptr, framebuffer });
    }

    void onResize(const glm::u32vec2& size) override {
        mGeometryBuffer.resize(size);
        mLitObjectsBuffer.resize(size);
    }

    nsm::SkyboxLayer* mLayerSkybox;
    nsm::ModelLayer* mLayerMain;
    nsm::LightingLayer* mLayerLightingDirectional;
    nsm::LightingLayer* mLayerLightingPoint;
    nsm::BloomLayer* mLayerBloom;
    nsm::TonemapLayer* mLayerTonemap;
    DebugLayer* mLayerDebug;
    nsm::ImGuiLayer* mLayerImGui;

    nsm::Framebuffer mGeometryBuffer;
    nsm::Framebuffer mLitObjectsBuffer;

    nsm::ShaderProgram mBlitShader;
    nsm::RenderState mBlitRenderState;

    bool mFirstFrame = true;
};

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
    {
        mGraphics.getRenderer().usePipeline<DemoPipeline>();
        mGraphics.getWindow().setVsync(false);
    }

    void onUpdate(const f32 timeStep) override {
        ImGui::ShowDemoWindow();

        mGraphics.getWindow().setTitle("Nasmo Sandbox | " + std::to_string(1.0f / timeStep) + " FPS");
    }
};

int main(int argc, char** argv) {
    auto info = nsm::Application::ApplicationInfo(argc, argv);
    info.graphics.window = {
        .title = "Nasmo Sandbox",
        .size = {1280, 720},
        .type = nsm::Window::WindowType::Windowed
    };
    info.initialScene = "scenes/demo.json";

    SandboxApplication app(info);
    app.run();    

    return 0;
}
