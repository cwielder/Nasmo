#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class DeccerModel {
public:
    struct InstanceData {
        glm::mat4 transform;
    };

    static inline const std::pair<std::string, std::size_t> sObjectInstanceDataSizes[] = {
        { "Cube", sizeof(InstanceData) },
        { "Cube.001", sizeof(InstanceData) },
        { "Cube.002", sizeof(InstanceData) },
        { "Cube.003", sizeof(InstanceData) },
        { "Cube.004", sizeof(InstanceData) },
        { "Cube.005", sizeof(InstanceData) },
        { "Cube.006", sizeof(InstanceData) },
        { "Cube.007", sizeof(InstanceData) },
        { "Cube.008", sizeof(InstanceData) },
        { "Cube.009", sizeof(InstanceData) },
        { "Cube.010", sizeof(InstanceData) },
        { "Cube.011", sizeof(InstanceData) },
        { "Cube.012", sizeof(InstanceData) },
        { "Cube.013", sizeof(InstanceData) },
        { "Cube.014", sizeof(InstanceData) },
        { "Cube.015", sizeof(InstanceData) },
        { "Cube.016", sizeof(InstanceData) },
        { "Cube.017", sizeof(InstanceData) },
        { "Cube.018", sizeof(InstanceData) }
    };

public:
    DeccerModel(nsm::Entity* owner)
        : mModelComponent(new nsm::ModelComponent("models/deccer.gltf", "main", sObjectInstanceDataSizes))
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
        mModelComponent->setInstanceData("Cube", &mInstanceData);
        mModelComponent->setInstanceData("Cube.001", &mInstanceData);
        mModelComponent->setInstanceData("Cube.002", &mInstanceData);
        mModelComponent->setInstanceData("Cube.003", &mInstanceData);
        mModelComponent->setInstanceData("Cube.004", &mInstanceData);
        mModelComponent->setInstanceData("Cube.005", &mInstanceData);
        mModelComponent->setInstanceData("Cube.006", &mInstanceData);
        mModelComponent->setInstanceData("Cube.007", &mInstanceData);
        mModelComponent->setInstanceData("Cube.008", &mInstanceData);
        mModelComponent->setInstanceData("Cube.009", &mInstanceData);
        mModelComponent->setInstanceData("Cube.010", &mInstanceData);
        mModelComponent->setInstanceData("Cube.011", &mInstanceData);
        mModelComponent->setInstanceData("Cube.012", &mInstanceData);
        mModelComponent->setInstanceData("Cube.013", &mInstanceData);
        mModelComponent->setInstanceData("Cube.014", &mInstanceData);
        mModelComponent->setInstanceData("Cube.015", &mInstanceData);
        mModelComponent->setInstanceData("Cube.016", &mInstanceData);
        mModelComponent->setInstanceData("Cube.017", &mInstanceData);
        mModelComponent->setInstanceData("Cube.018", &mInstanceData);
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
    DeccerModel mModel;
};

NSM_REGISTER_ENTITY(DemoEntity);
