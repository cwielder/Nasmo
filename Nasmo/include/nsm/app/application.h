#pragma once

#include <nsm/common.h>
#include <nsm/gfx/window.h>

#include <string>
#include <vector>

int main(int argc, char** argv);

namespace nsm {

    class Application {
    public:
        struct ApplicationInfo {
            ApplicationInfo(int argc, char** argv)
                : args(argc)
            {
                for (int i = 0; i < argc; ++i) {
                    this->args[i] = argv[i];
                }

                this->window.title = "NSM Window";
                this->window.size = glm::u32vec2(1280, 720);
                this->window.type = nsm::Window::WindowType::Windowed;
            }

            std::vector<std::string> args;
            nsm::Window::WindowInfo window;
        };

    public:
        Application(const ApplicationInfo& info);
        virtual ~Application();

    private:
        friend int ::main(int argc, char** argv);

        void run();

        nsm::Window mWindow;
    };

}
