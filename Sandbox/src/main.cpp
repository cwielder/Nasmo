#include <iostream>
#include <nsm/app/application.h>
#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/layerstack.h>

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
    {
        mGraphics.getLayerStack().pushLayer<nsm::ModelLayer>("main");
        mGraphics.getWindow().setVsync(false);
    }

    void onUpdate(const f32 timeStep) override {
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
    info.initialScene = "scene.json";

    SandboxApplication app(info);
    app.run();    

    return 0;
}
