#include <nsm/entity/entity.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/graphics.h>
#include <nsm/debug/log.h>

class CameraEntity : public nsm::Entity {
public:
    CameraEntity(const nsm::Entity::Properties&)
        : mPosition() // TODO: Set position from properties
    { }

    ~CameraEntity() override = default;

    void onCreate() override {
        const f32 aspectRatio = static_cast<f32>(nsm::Graphics::getFramebufferSize().x) / nsm::Graphics::getFramebufferSize().y;

        nsm::CameraComponent* cameraComponent = new nsm::OrthographicCameraComponent(
            mPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f, -1.0f, aspectRatio, -aspectRatio, -1.0f, 1.0f
        );
        cameraComponent->setTargetLayer("main");

        this->addComponent<nsm::CameraComponent>(cameraComponent);
    }

    void onUpdate(const f32 timeStep) override {
        this->getComponents<nsm::CameraComponent>()[0]->setView(mPosition, glm::vec3(mPosition.x, mPosition.y, 1.0f));
    }

    glm::vec3 mPosition;
};

NSM_REGISTER_ENTITY(CameraEntity);
