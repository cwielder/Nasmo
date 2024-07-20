#include <nsm/app/application.h>
#include <nsm/gfx/layer/layerstack.h>
#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/bloomlayer.h>
#include <nsm/gfx/layer/tonemaplayer.h>
#include <nsm/gfx/layer/imguilayer.h>
#include <nsm/entity/component/modelcomponent.h>

#include <unordered_map>

#include <imgui.h>

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
    {
        mGraphics.getLayerStack().pushLayer<nsm::ModelLayer>("main");
        mGraphics.getLayerStack().pushLayer<nsm::BloomLayer>("bloom");
        mGraphics.getLayerStack().pushLayer<nsm::TonemapLayer>("cc");
        mGraphics.getLayerStack().pushLayer<nsm::ImGuiLayer>("imgui");

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
