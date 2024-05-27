#include <nsm/app/application.h>

#include <nsm/gfx/opengl.h>

nsm::Application::Application(const ApplicationInfo& info)
    : mWindow(info.window)
{

}

nsm::Application::~Application() { }

void nsm::Application::run() {
    while (mWindow.update()) {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
