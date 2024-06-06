#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <glm/gtc/matrix_transform.hpp>

class DemoEntity final : public nsm::Entity {
    struct ModelData {
        glm::mat4 transform;
    };

public:
    static constexpr int cInstanceCount = 1;

    DemoEntity(nsm::Entity::Properties&)
        : mModelData()
    { }

    ~DemoEntity() override = default;

    void onCreate() override {
        const std::pair<std::string, std::size_t> meshInstanceDataSizes[] = {
            { "sword", sizeof(ModelData) }
        };

        for (int i = 0; i < cInstanceCount; i++) {
            mModelData[i].transform = glm::translate(glm::mat4(1.0f), glm::vec3(
                (rand() % 100) - 50,
                (rand() % 100) - 50,
                (rand() % 100) - 50
            ) * glm::vec3(cInstanceCount > 1));

            nsm::ModelComponent* mc = new nsm::ModelComponent("models/sword.json", "main", meshInstanceDataSizes);
            mc->setInstanceData("sword", &mModelData[i]);
            this->addComponent<nsm::DrawableComponent>(mc);
        }
    }

    void onUpdate(const f32 timeStep) override {
        for (int i = 0; i < cInstanceCount; i++) {
            mModelData[i].transform = glm::rotate(mModelData[i].transform, timeStep , glm::vec3(1.0f));

            nsm::ModelComponent* mc = static_cast<nsm::ModelComponent*>(this->getComponents<nsm::DrawableComponent>()[i]);
            mc->setInstanceData("sword", &mModelData[i]);
        }
    }

    ModelData mModelData[cInstanceCount];
};

NSM_REGISTER_ENTITY(DemoEntity);
