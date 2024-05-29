#include <iostream>
#include <nsm/app/application.h>

int main(int argc, char** argv) {
    auto info = nsm::Application::ApplicationInfo(argc, argv);
    info.graphics.window = {
        .title = "Nasmo Sandbox",
        .size = {1280, 720},
        .type = nsm::Window::WindowType::Windowed
    };
    info.initialScene = "scene.json";

    nsm::Application app(info);
    app.run();    

    return 0;
}
