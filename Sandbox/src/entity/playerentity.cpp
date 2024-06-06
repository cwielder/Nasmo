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
    static constexpr int cInstanceCount = 100;

    PlayerEntity(nsm::Entity::Properties&)
        : mModelData()
    { }

    ~PlayerEntity() override = default;

    void onCreate() override {
        const std::pair<std::string, std::size_t> meshInstanceDataSizes[] = {
            { "skull", sizeof(ModelData) }
        };

        for (int i = 0; i < cInstanceCount; i++) {
            // randomize position between -100 and 100
            mModelData[i].transform = glm::translate(glm::mat4(1.0f), glm::vec3(
                (rand() % 100) - 50,
                (rand() % 100) - 50,
                (rand() % 100) - 50
            ));
            // scale by 0.1
            mModelData[i].transform = glm::scale(mModelData[i].transform, glm::vec3(0.1f));

            nsm::ModelComponent* mc = new nsm::ModelComponent("skull.json", "main", meshInstanceDataSizes);
            mc->setInstanceData("skull", &mModelData[i]);
            this->addComponent<nsm::DrawableComponent>(mc);
        }
    }

    void onUpdate(const f32 timeStep) override {
        for (int i = 0; i < cInstanceCount; i++) {
            // rotate by 0.1 degrees around the y-axis
            mModelData[i].transform = glm::rotate(mModelData[i].transform, timeStep, glm::vec3(0.0f, 1.0f, 0.0f));

            nsm::ModelComponent* mc = static_cast<nsm::ModelComponent*>(this->getComponents<nsm::DrawableComponent>()[i]);
            mc->setInstanceData("skull", &mModelData[i]);
        }
    }

    ModelData mModelData[cInstanceCount];
};

NSM_REGISTER_ENTITY(PlayerEntity);
