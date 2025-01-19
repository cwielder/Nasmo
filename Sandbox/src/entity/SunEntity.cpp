#include "entity/SunEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/util/Color.h>
#include <imgui.h>

SunEntity::SunEntity(nsm::Entity::Properties &properties)
    : mIntensity(nsm::JsonHelpers::getFloat(properties, "intensity"))
    , mYaw(nsm::JsonHelpers::getFloat(properties, "yaw"))
    , mPitch(nsm::JsonHelpers::getFloat(properties, "pitch"))
    , mLightComponent(nullptr)
{ }

void SunEntity::onCreate(nsm::Entity::Properties &) {
    mLightComponent = new nsm::DirectionalLightComponent(
        mYaw, mPitch,
        glm::vec3(1.0f, 1.0f, 1.0f),
        mIntensity);
    mLightComponent->setTargetLayer("lighting_directional");
    this->addComponent<nsm::DrawableComponent>(mLightComponent);

    mSkyboxComponent = new nsm::SkyboxComponent("textures/skybox1/", ".jpg");
    mSkyboxComponent->setTargetLayer("skybox");
    this->addComponent<nsm::DrawableComponent>(mSkyboxComponent);

    mAmbientLightComponent = new nsm::AmbientLightComponent(0.05f, nsm::Color{"#FFD8B5"}.getRGBA());
    mAmbientLightComponent->setTargetLayer("lighting_directional");
    this->addComponent<nsm::DrawableComponent>(mAmbientLightComponent);
}

void SunEntity::onUpdate(const f64) {
    static f32 intensity = mIntensity;
    static f32 yaw = mYaw;
    static f32 pitch = mPitch;

    static f32 ambientIntensity = mAmbientLightComponent->getIntensity();
    static glm::vec3 ambientColor = mAmbientLightComponent->getColor();

    if (ImGui::Begin("Sun Entity")) {
        ImGui::DragFloat("Intensity", &intensity, 0.1f);
        ImGui::DragFloat("Yaw", &yaw, 0.1f);
        ImGui::DragFloat("Pitch", &pitch, 0.1f);
        ImGui::Separator();
        ImGui::DragFloat("Ambient Intensity", &ambientIntensity, 0.1f);
        ImGui::ColorEdit3("Ambient Color", &ambientColor.x);
    } ImGui::End();

    mLightComponent->setIntensity(intensity);
    mLightComponent->setDirection(yaw, pitch);

    mAmbientLightComponent->setIntensity(ambientIntensity);
    mAmbientLightComponent->setColor(ambientColor);
}

NSM_REGISTER_ENTITY(SunEntity);
