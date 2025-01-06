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
        mTargetPosition = playerPosition + sOffset;
        const glm::vec3 initialPosition = playerPosition + glm::vec3(-328.0f, 128.0f, 0.0f);

        mTransform = new nsm::TransformComponent(initialPosition);
        this->addComponent<nsm::TransformComponent>(mTransform);

        nsm::CameraComponent* cameraComponent = new nsm::PerspectiveCameraComponent(
            initialPosition,
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
        static f32 followSpeed = 2.0f;

        if (ImGui::Begin("Camera")) {
            ImGui::DragFloat3("Position", &sOffset.x, 0.1f);
            ImGui::DragFloat("Follow Speed", &followSpeed, 0.01f, 0.0f, 1.0f);
        } ImGui::End();

        mTargetPosition = position + sOffset;

        glm::vec3 currentPosition = mTransform->getPosition();
        glm::vec3 displacement = mTargetPosition - currentPosition;
        glm::vec3 smoothedStep = displacement * (1.0f - glm::exp(-followSpeed * static_cast<f32>(timeStep)));

        // Update the position
        currentPosition += smoothedStep;

        mTransform->setPosition(currentPosition);

        mCamera->setView(mTransform->getPosition(), (mTransform->getPosition()) + glm::vec3(1.0f, -glm::sin(glm::radians(45.0f)), 0.0f));
    }

private:
    static inline glm::vec3 sOffset = glm::vec3(-64.0f, 64.0f, 0.0f);

private:
    nsm::TransformComponent* mTransform;
    nsm::CameraComponent* mCamera;
    nsm::Entity* mPlayerEntity;
    glm::vec3 mTargetPosition;
};

NSM_REGISTER_ENTITY(CameraEntity);
