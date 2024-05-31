#include <iostream>
#include <nsm/app/application.h>
#include <nsm/gfx/layer.h>

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
    {
        mGraphics.getLayerStack().pushLayer<nsm::Layer>("main");
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
