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

    mAmbientLightComponent = new nsm::AmbientLightComponent(0.4f, nsm::Color{"#FFD8B5"}.getRGBA());
    mAmbientLightComponent->setTargetLayer("lighting_directional");
    this->addComponent<nsm::DrawableComponent>(mAmbientLightComponent);
}

NSM_REGISTER_ENTITY(SunEntity);
