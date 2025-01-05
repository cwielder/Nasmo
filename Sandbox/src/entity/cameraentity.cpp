#include <nsm/entity/entity.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/gfx/graphics.h>
#include <nsm/debug/log.h>
#include <nsm/util/jsonhelpers.h>
#include <nsm/entity/scene.h>
#include <imgui.h>

class CameraEntity : public nsm::Entity {
public:
    CameraEntity(nsm::Entity::Properties&)
        : mTransform(nullptr)
        , mPlayerEntity(nullptr)
    { }

    ~CameraEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        const f32 aspectRatio = static_cast<f32>(nsm::Graphics::getFramebufferSize().x) / nsm::Graphics::getFramebufferSize().y;

        const std::vector<nsm::Entity*>& entities = mScene->getEntities();
        for (nsm::Entity* entity : entities) {
            if (entity->getIdentifier() == "PlayerEntity") {
                mPlayerEntity = entity;
                break;
            }
        }

        NSM_ASSERT(mPlayerEntity != nullptr, "Player entity not found!");

        const glm::vec3 playerPosition = mPlayerEntity->getComponents<nsm::TransformComponent>()[0]->getPosition();

        mTransform = new nsm::TransformComponent(playerPosition + glm::vec3(5.0f, 2.0f, 0.0f));
        this->addComponent<nsm::TransformComponent>(mTransform);

        nsm::CameraComponent* cameraComponent = new nsm::PerspectiveCameraComponent(
            mTransform->getPosition(),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            45.0f,
            aspectRatio,
            0.01f,
            10000.0f
        );
        cameraComponent->addTargetLayer("main");
        cameraComponent->addTargetLayer("forward");

        this->addComponent<nsm::CameraComponent>(cameraComponent);
        mCamera = cameraComponent;
    }

    void onUpdate(const f64 timeStep) override {
        const glm::vec3 position = mPlayerEntity->getComponents<nsm::TransformComponent>()[0]->getPosition();
        static glm::vec3 offset = glm::vec3(-56.0f, 52.0f, 0.0f);

        if (ImGui::Begin("Camera")) {
            ImGui::DragFloat3("Position", &offset.x, 0.1f);
        } ImGui::End();

        mTransform->setPosition(position + offset);

        mCamera->setView(mTransform->getPosition(), (position + offset) + glm::vec3(1.0f, -glm::sin(glm::radians(45.0f)), 0.0f));
    }

private:
    nsm::TransformComponent* mTransform;
    nsm::CameraComponent* mCamera;
    nsm::Entity* mPlayerEntity;
};

NSM_REGISTER_ENTITY(CameraEntity);
