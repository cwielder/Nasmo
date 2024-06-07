#include <nsm/entity/entity.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/graphics.h>
#include <nsm/debug/log.h>
#include <nsm/util/jsonhelpers.h>

class CameraEntity : public nsm::Entity {
public:
    CameraEntity(nsm::Entity::Properties& properties)
        : mPosition(nsm::JsonHelpers::getVec3(properties, "position"))
        , mTime(0.0f)
    { }

    ~CameraEntity() override = default;

    void onCreate() override {
        const f32 aspectRatio = static_cast<f32>(nsm::Graphics::getFramebufferSize().x) / nsm::Graphics::getFramebufferSize().y;

        nsm::CameraComponent* cameraComponent = new nsm::PerspectiveCameraComponent(
            mPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            45.0f,
            aspectRatio,
            0.01f,
            1000.0f
        );
        cameraComponent->setTargetLayer("main");

        this->addComponent<nsm::CameraComponent>(cameraComponent);
    }

    void onUpdate(const f32 timeStep) override {
        mPosition.x = static_cast<f32>(std::sin(mTime) * 20.0);
        mTime += timeStep;

        this->getComponents<nsm::CameraComponent>()[0]->setView(mPosition, glm::vec3(mPosition.x, mPosition.y, 1.0f));
    }

    glm::vec3 mPosition;
    f64 mTime;
};

NSM_REGISTER_ENTITY(CameraEntity);
