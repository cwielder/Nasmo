#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <glm/gtc/matrix_transform.hpp>

class PlayerEntity : public nsm::Entity {
    struct ModelData {
        glm::mat4 transform;
    };

public:
    PlayerEntity(nsm::Entity::Properties&) { }
    ~PlayerEntity() override = default;

    void onCreate() override {
        mScale = glm::vec3(0.5f);

        nsm::ModelComponent* modelComponent = new nsm::ModelComponent("skull.json", "main");
        modelComponent->setInstanceData<ModelData>("skull", &mModelData);
        this->addComponent<nsm::DrawableComponent>(modelComponent);
    }

    void onUpdate(const f32 timeStep) override {
        mRotation.y += timeStep;

        mModelData.transform = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

        nsm::ModelComponent* modelComponent = static_cast<nsm::ModelComponent*>(this->getComponents<nsm::DrawableComponent>()[0]);
        modelComponent->setInstanceDataDirty<ModelData>("skull");
    }

    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::vec3 mScale;
    ModelData mModelData;
};

NSM_REGISTER_ENTITY(PlayerEntity);
