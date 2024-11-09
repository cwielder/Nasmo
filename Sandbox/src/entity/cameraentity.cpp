#include <nsm/entity/entity.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/gfx/graphics.h>
#include <nsm/debug/log.h>
#include <nsm/util/jsonhelpers.h>
#include <imgui.h>

class CameraEntity : public nsm::Entity {
public:
    CameraEntity(nsm::Entity::Properties&)
        : mTransform(nullptr)
        , mTime(0.0f)
    { }

    ~CameraEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        const f32 aspectRatio = static_cast<f32>(nsm::Graphics::getFramebufferSize().x) / nsm::Graphics::getFramebufferSize().y;

        mTransform = new nsm::TransformComponent(nsm::JsonHelpers::getVec3(properties, "position"));
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
        cameraComponent->addTargetLayer("debug");

        this->addComponent<nsm::CameraComponent>(cameraComponent);
    }

    void onUpdate(const f32 timeStep) override {
        glm::vec3 position = mTransform->getPosition();

        if (ImGui::Begin("Camera Controls")) {
            ImGui::DragFloat3("Position", &position.x, 0.1f);
        } ImGui::End();

        mTransform->setPosition(position);

        mTime += timeStep;

        //mPosition.x = static_cast<f32>(10.0f * std::cos(mTime));
        //mPosition.y = static_cast<f32>(5.0f * std::sin(mTime));

        this->getComponents<nsm::CameraComponent>()[0]->setView(mTransform->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    nsm::TransformComponent* mTransform;
    f64 mTime;
};

NSM_REGISTER_ENTITY(CameraEntity);
