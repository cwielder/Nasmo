#include <nsm/entity/entity.h>
#include <nsm/entity/component/lightcomponent.h>
#include <nsm/util/jsonhelpers.h>
#include <imgui.h>
#include <glm/trigonometric.hpp>
#include <nsm/gfx/primitiveshape.h>
#include <glm/gtc/matrix_transform.hpp>
#include <nsm/gfx/renderinfo.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/entity/component/skyboxcomponent.h>

class SunEntity final : public nsm::Entity {
public:
    SunEntity(nsm::Entity::Properties& properties)
        : mIntensity(nsm::JsonHelpers::getFloat(properties, "intensity"))
        , mYaw(nsm::JsonHelpers::getFloat(properties, "yaw"))
        , mPitch(nsm::JsonHelpers::getFloat(properties, "pitch"))
        , mLightComponent(nullptr)
    { }

    ~SunEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override {
        mLightComponent = new nsm::DirectionalLightComponent(
            mYaw, mPitch,
            glm::vec3(1.0f, 1.0f, 1.0f),
            mIntensity
        );
        mLightComponent->setTargetLayer("lighting_directional");
        this->addComponent<nsm::DrawableComponent>(mLightComponent);

        mSkyboxComponent = new nsm::SkyboxComponent("textures/skybox1/", ".jpg");
        mSkyboxComponent->setTargetLayer("skybox");
        this->addComponent<nsm::DrawableComponent>(mSkyboxComponent);
    }

    void onUpdate(const f64 timeStep) override {
        static f32 colorR = 1.0f, colorG = 1.0f, colorB = 1.0f;
        static f32 intensity = mIntensity;
        static f32 yaw = 0.0f, pitch = -124.0f;

        static f32 indicatorLength = 1.5f;

        if (ImGui::Begin("Sun")) {
            ImGui::SliderFloat("Yaw", &yaw, 0.0f, 180.0f);
            ImGui::SliderFloat("Pitch", &pitch, 0.0f, 360.0f);
            ImGui::SliderFloat("Color R", &colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &colorB, 0.0f, 1.0f);
            ImGui::DragFloat("Intensity", &intensity, 0.1f);
            ImGui::DragFloat("Indicator Length", &indicatorLength, 0.1f);
        } ImGui::End();

        mLightComponent->setColor(glm::vec3(colorR, colorG, colorB));
        mLightComponent->setIntensity(intensity);
        mLightComponent->setDirection(yaw, pitch);
    }

private:
    f32 mIntensity, mYaw, mPitch;
    nsm::DirectionalLightComponent* mLightComponent;
    nsm::SkyboxComponent* mSkyboxComponent;
};

NSM_REGISTER_ENTITY(SunEntity);
