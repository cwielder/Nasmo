#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class PlaneModel {
public:
    struct InstanceData {
        glm::mat4 transform;
    };

    static inline const std::pair<std::string, std::size_t> sObjectInstanceDataSizes[] = {
        { "Cylinder064_13 - Default_0", sizeof(InstanceData) }
    };

public:
    PlaneModel(nsm::Entity* owner)
        : mModelComponent(new nsm::ModelComponent("models/plane.glb", "main", sObjectInstanceDataSizes))
        , mInstanceData({
            .transform = glm::mat4(1.0f)
        })
    {
        this->updateInstanceData();
        owner->addComponent<nsm::DrawableComponent>(mModelComponent);
    }

    void setScale(const glm::vec3& scale) {
        mInstanceData.transform = glm::scale(glm::mat4(1.0f), scale);

        this->updateInstanceData();
    }

private:
    void updateInstanceData() {
        mModelComponent->setInstanceData("Cylinder064_13 - Default_0", &mInstanceData);
    }

    nsm::ModelComponent* mModelComponent;
    InstanceData mInstanceData;
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
