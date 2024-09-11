#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class PlaneModel {
public:
    PlaneModel(nsm::Entity* owner)
        : mModelComponent(new nsm::ModelComponent("models/plane.glb", "main"))
    {
        owner->addComponent<nsm::DrawableComponent>(mModelComponent);
    }

    void setScale(const glm::vec3& scale) {
        glm::mat4 transform = glm::scale(glm::mat4(1.0f), scale);
        mModelComponent->setTransformAll(transform);
    }

private:
    nsm::ModelComponent* mModelComponent;
};

class DemoEntity final : public nsm::Entity {
public:
    DemoEntity(nsm::Entity::Properties&)
        : mScale(1.0f)
        , mModel(this)
    { }

    ~DemoEntity() override = default;

    void onCreate() override {

    }

    void onUpdate(const f32 timeStep) override {
        if (ImGui::Begin("Demo Entity")) {
            ImGui::DragFloat("Scale", &mScale, 0.1f);
        } ImGui::End();

        mModel.setScale(glm::vec3(mScale));
    }

private:
    f32 mScale;
    PlaneModel mModel;
};

NSM_REGISTER_ENTITY(DemoEntity);
