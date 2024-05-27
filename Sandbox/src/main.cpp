#include <iostream>
#include <nsm/app/application.h>

int main(int argc, char** argv) {
    auto info = nsm::Application::ApplicationInfo(argc, argv);
    info.window = {
        .title = "Nasmo Sandbox",
        .size = {1280, 720},
        .type = nsm::Window::WindowType::Windowed
    };

    nsm::Application app(info);
    app.run();    

    return 0;
}
