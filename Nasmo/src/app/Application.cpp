#include <nsm/app/Application.h>

#include <nsm/gfx/OpenGL.h>
#include <nsm/event/Events.h>
#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/entity/component/CameraComponent.h>
#include <nsm/gfx/layer/Layer.h>

#include <tracy/Tracy.hpp>

std::deque<const nsm::Event*> nsm::Application::sEventQueue;

nsm::Application::Application(const ApplicationInfo& info)
    : mGraphics(info.graphics)
    , mScene(info.initialScene)
    , mAudio(info.audio)
{
    TracyNoop;
}

nsm::Application::~Application() { }

void nsm::Application::run() {
    this->intermoduleDataTransfer();

    mDeltaTime.update();

    do { // Main loop
        const f64 ts = mDeltaTime.update();

        this->onUpdate(ts);
        this->handleEvents();

        mScene.update(ts);
        mAudio.update();

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
    mAudio.transferData(mScene.getEntities());

    this->onSystemTransferData();
}
