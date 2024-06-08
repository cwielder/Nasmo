#include <nsm/app/application.h>
#include <nsm/gfx/layer/modellayer.h>
#include <nsm/gfx/layer/layerstack.h>
#include <nsm/gfx/layer/imguilayer.h>
#include <nsm/entity/component/modelcomponent.h>

#include <unordered_map>

#include <imgui.h>

class DebugSystem {
public:
    void transferData(const std::vector<nsm::Entity*>& entities) {
        for (auto entity : entities) {
            auto drawableComponents = entity->getComponents<nsm::DrawableComponent>();
            for (auto drawableComponent : drawableComponents) {
                if (dynamic_cast<nsm::ModelComponent*>(drawableComponent)) {
                    mModels[static_cast<nsm::ModelComponent*>(drawableComponent)->getModel()]++;
                }
            }
        }
    }

    void update() {
        if (ImGui::Begin("Debug")) {
            for (auto models : mModels) {
                for (auto& mesh : models.first->getMeshes()) {
                    ImGui::Text("%s/%s: %zu instances * %zu indices = %zu vertices", models.first->getPath().c_str(), mesh.first.c_str(), models.second, mesh.second->getIndexBuffer().getCount(), models.second * mesh.second->getIndexBuffer().getCount());
                }
            }
        } ImGui::End();

        mModels.clear();
    }

private:
    std::unordered_map<nsm::Model*, std::size_t> mModels;
};

class SandboxApplication : public nsm::Application {
public:
    SandboxApplication(const ApplicationInfo& info)
        : Application(info)
        , mDebugSystem()
    {
        mGraphics.getLayerStack().pushLayer<nsm::ModelLayer>("main");
        mGraphics.getLayerStack().pushLayer<nsm::ImGuiLayer>("imgui");

        mGraphics.getWindow().setVsync(false);
    }

    void onUpdate(const f32 timeStep) override {
        ImGui::ShowDemoWindow();

        mDebugSystem.update();

        mGraphics.getWindow().setTitle("Nasmo Sandbox | " + std::to_string(1.0f / timeStep) + " FPS");
    }

    void onSystemTransferData() override {
        mDebugSystem.transferData(mScene.getEntities());
    }

    DebugSystem mDebugSystem;
};

int main(int argc, char** argv) {
    auto info = nsm::Application::ApplicationInfo(argc, argv);
    info.graphics.window = {
        .title = "Nasmo Sandbox",
        .size = {1280, 720},
        .type = nsm::Window::WindowType::Windowed
    };
    info.initialScene = "scenes/demo.json";

    SandboxApplication app(info);
    app.run();    

    return 0;
}
