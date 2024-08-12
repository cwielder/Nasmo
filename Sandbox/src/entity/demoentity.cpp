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
        , mRandsX()
        , mRandsY()
        , mRandsZ()
    { }

    ~DemoEntity() override = default;

    void onCreate() override {
        const std::pair<std::string, std::size_t> meshInstanceDataSizes[] = {
            { "Cube", sizeof(ModelData) },
            { "Cube.001", sizeof(ModelData) },
            { "Cube.002", sizeof(ModelData) },
            { "Cube.003", sizeof(ModelData) },
            { "Cube.004", sizeof(ModelData) }
        };

        mModelData[0].transform = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
        nsm::ModelComponent* mc = new nsm::ModelComponent("models/deccer.gltf", "main", meshInstanceDataSizes);
        mc->setInstanceData("Cube", &mModelData[0]);
        mc->setInstanceData("Cube.001", &mModelData[0]);
        mc->setInstanceData("Cube.002", &mModelData[0]);
        mc->setInstanceData("Cube.003", &mModelData[0]);
        mc->setInstanceData("Cube.004", &mModelData[0]);
        this->addComponent<nsm::DrawableComponent>(mc);

        //for (int i = 0; i < cInstanceCount; i++) {
        //    mModelData[i].transform = glm::translate(glm::mat4(1.0f), glm::vec3(
        //        (rand() % 100) - 50,
        //        (rand() % 100) - 50,
        //        (rand() % 100) - 50
        //    ) * glm::vec3(cInstanceCount > 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
//
        //    nsm::ModelComponent* mc = new nsm::ModelComponent("models/Avocado.glb", "main", meshInstanceDataSizes);
        //    mc->setInstanceData("Avocado", &mModelData[i]);
        //    this->addComponent<nsm::DrawableComponent>(mc);
        //}

        for (int i = 0; i < cInstanceCount; i++) {
            mRandsX[i] = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);
            mRandsY[i] = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);
            mRandsZ[i] = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);
        }
    }

    void onUpdate(const f32 timeStep) override {
        //for (int i = 0; i < cInstanceCount; i++) {
        //    mModelData[i].transform = glm::rotate(
        //        mModelData[i].transform, timeStep * mRandsX[i], glm::vec3(
        //        (mRandsX[i] * 50.0f) - 25.0f,
        //        (mRandsY[i] * 50.0f) - 25.0f,
        //        (mRandsZ[i] * 50.0f) - 25.0f
        //    ));
//
        //    nsm::ModelComponent* mc = static_cast<nsm::ModelComponent*>(this->getComponents<nsm::DrawableComponent>()[i]);
        //    mc->setInstanceData("Avocado", &mModelData[i]);
        //}
    }

    ModelData mModelData[cInstanceCount];
    f32 mRandsX[cInstanceCount], mRandsY[cInstanceCount], mRandsZ[cInstanceCount];
};

NSM_REGISTER_ENTITY(DemoEntity);
