#include <nsm/app/application.h>

#include <nsm/gfx/opengl.h>

nsm::Application::Application(const ApplicationInfo& info)
    : mGraphics(info.graphics)
    , mScene(info.initialScene)
{ }

nsm::Application::~Application() { }

#include <nsm/gfx/shader.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>

void nsm::Application::run() {
    //this->intermoduleDataTransfer();

    do { // Main loop
        const f32 ts = mGraphics.getTimeStep();

        this->onUpdate(ts);
        //this->handleEvents();

        mScene.update(ts);

        //this->intermoduleDataTransfer();
    } while (mGraphics.update());
}
