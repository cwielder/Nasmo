#include <nsm/app/Application.h>
#include <nsm/gfx/Renderer.h>
#include <nsm/gfx/pipeline/StandardPipeline.h>

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
    {
        mGraphics.getRenderer().usePipeline<nsm::StandardPipeline>();
        mGraphics.getWindow().setVsync(false);
    }

    void onUpdate(const f64 timeStep) override {
        mTime += timeStep;
        mFrames++;

        if (mTime >= 1.0) {
            mGraphics.getWindow().setTitle("Nasmo Sandbox - " + std::to_string(mFrames) + " FPS");
            
            mTime = 0.0;
            mFrames = 0;
        }
    }

private:
    f64 mTime = 0.0;
    u32 mFrames = 0;
};

int main(int argc, char** argv) {
    auto info = nsm::Application::ApplicationInfo(argc, argv);
    info.graphics.window = {
        .title = "Nasmo Sandbox",
        .size = {1920, 1080},
        .type = nsm::Window::WindowType::Windowed //! TODO: Fullscreen is broken with framebuffers
    };
    info.initialScene = "scenes/demo.json";

    SandboxApplication app(info);
    app.run();    

    return 0;
}
