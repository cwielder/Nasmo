#include <nsm/app/application.h>

#include <nsm/gfx/opengl.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/layer/layer.h>

std::deque<const nsm::Event*> nsm::Application::sEventQueue;

nsm::Application::Application(const ApplicationInfo& info)
    : mGraphics(info.graphics)
    , mScene(info.initialScene)
{ }

nsm::Application::~Application() { }

void nsm::Application::run() {
    this->intermoduleDataTransfer();

    do { // Main loop
        const f64 ts = mDeltaTime.update();

        this->onUpdate(ts);
        this->handleEvents();

        mScene.update(ts);

        this->intermoduleDataTransfer();
    } while (mGraphics.update());
}

void nsm::Application::handleEvents() {
    std::deque<const Event*> eventQueueCopy;
    eventQueueCopy.swap(sEventQueue);

    for (auto& event : eventQueueCopy) {
        this->onEventInternal(event);
        this->onEvent(event); // User-defined event handling

        mScene.onEvent(event);
        mGraphics.onEvent(event);

        delete event;
    }
}

void nsm::Application::onEventInternal(const Event* event) {
    
}

void nsm::Application::raiseEvent(const Event* event) {
    sEventQueue.push_back(event);
}

void nsm::Application::intermoduleDataTransfer() {
    mGraphics.transferData(mScene.getEntities());

    this->onSystemTransferData();
}
