#include <nsm/app/application.h>
#include <nsm/gfx/renderer.h>
#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/bloomlayer.h>
#include <nsm/gfx/layer/tonemaplayer.h>
#include <nsm/gfx/layer/imguilayer.h>
#include <nsm/gfx/layer/lightinglayer.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/gfx/renderpipeline.h>

#include <unordered_map>

#include <imgui.h>

class DemoPipeline : public nsm::RenderPipeline {
public:
    DemoPipeline() {
        mLayerMain = this->pushLayer<nsm::ModelLayer>("main");
        mLayerLighting = this->pushLayer<nsm::LightingLayer>("lighting");
        mLayerBloom = this->pushLayer<nsm::BloomLayer>("bloom");
        mLayerTonemap = this->pushLayer<nsm::TonemapLayer>("cc");
        mLayerImGui = this->pushLayer<nsm::ImGuiLayer>("imgui");

        glm::u32vec2 size = nsm::Graphics::getFramebufferSize();
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // normal+metallic
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::RGBA16F, size); // albedo+roughness
        mGeometryBuffer.addTextureBuffer(nsm::Texture::Format::Depth24Stencil8, size);
        mGeometryBuffer.finalize();
    }

    void render(const nsm::Framebuffer* framebuffer) override {
        // Build geometry buffer
        mGeometryBuffer.bind();
        mGeometryBuffer.clear(glm::f32vec4{ 0.0f }, nsm::Framebuffer::Type::Color, 0);
        mGeometryBuffer.clear(glm::f32vec4{ 0.0f }, nsm::Framebuffer::Type::Color, 1);
        mGeometryBuffer.clear(glm::f32vec4{ 1.0f }, nsm::Framebuffer::Type::Depth, 2);
        mLayerMain->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

        // Lighting pass
        framebuffer->bind();
        mLayerLighting->draw({ mLayerMain->getCamera(), &mGeometryBuffer });

        // Post-process pass
        mLayerBloom->draw({ nullptr, framebuffer });
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        mLayerTonemap->draw({ nullptr, framebuffer });

        // ImGui pass
        mLayerImGui->draw({ nullptr, framebuffer });
    }

    nsm::ModelLayer* mLayerMain;
    nsm::LightingLayer* mLayerLighting;
    nsm::BloomLayer* mLayerBloom;
    nsm::TonemapLayer* mLayerTonemap;
    nsm::ImGuiLayer* mLayerImGui;

    nsm::Framebuffer mGeometryBuffer;
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
