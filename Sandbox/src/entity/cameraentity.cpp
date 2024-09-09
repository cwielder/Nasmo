#include <nsm/entity/entity.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/graphics.h>
#include <nsm/debug/log.h>
#include <nsm/util/jsonhelpers.h>
#include <imgui.h>

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
            10000.0f
        );
        cameraComponent->addTargetLayer("main");
        cameraComponent->addTargetLayer("debug");

        this->addComponent<nsm::CameraComponent>(cameraComponent);
    }

    void onUpdate(const f32 timeStep) override {
        if (ImGui::Begin("Camera Controls")) {
            ImGui::DragFloat3("Position", &mPosition.x, 0.1f);
        } ImGui::End();

        mTime += timeStep;

        //mPosition.x = static_cast<f32>(10.0f * std::cos(mTime));
        //mPosition.y = static_cast<f32>(5.0f * std::sin(mTime));

        this->getComponents<nsm::CameraComponent>()[0]->setView(mPosition, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 mPosition;
    f64 mTime;
};

NSM_REGISTER_ENTITY(CameraEntity);
