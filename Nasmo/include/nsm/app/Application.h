#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/Graphics.h>
#include <nsm/entity/Scene.h>
#include <nsm/app/DeltaTime.h>

#include <string>
#include <vector>
#include <deque>

int main(int argc, char** argv);

namespace nsm {

    class Event;

    class Application {
    public:
        struct ApplicationInfo {
            ApplicationInfo(int argc, char** argv)
                : args(argc)
            {
                for (int i = 0; i < argc; ++i) {
                    this->args[i] = argv[i];
                }

                this->graphics.window.title = "NSM Window";
                this->graphics.window.size = glm::u32vec2(1280, 720);
                this->graphics.window.type = nsm::Window::WindowType::Windowed;
            }

            std::vector<std::string> args;
            nsm::Graphics::GraphicsInfo graphics;
            std::string initialScene;
        };

    public:
        Application(const ApplicationInfo& info);
        virtual ~Application();

        virtual void onUpdate(const f64 timeStep) { }
        virtual void onEvent(const Event* event) { }
        virtual void onSystemTransferData() { }

        static void raiseEvent(const Event* event);

        nsm::Scene& getScene() { return mScene; }
        nsm::Graphics& getGraphics() { return mGraphics; }

    private:
        friend int ::main(int argc, char** argv);

        static std::deque<const Event*> sEventQueue;

        void run();
        void intermoduleDataTransfer();
        void handleEvents();
        void onEventInternal(const Event* event);

    protected:
        nsm::DeltaTime mDeltaTime;
        nsm::Graphics mGraphics;
        nsm::Scene mScene;
    };

}
