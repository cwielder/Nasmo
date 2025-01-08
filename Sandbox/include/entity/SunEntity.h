#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/LightComponent.h>
#include <nsm/entity/component/SkyboxComponent.h>

class SunEntity final : public nsm::Entity {
public:
    SunEntity(nsm::Entity::Properties& properties);
    ~SunEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override;

private:
    f32 mIntensity, mYaw, mPitch;
    nsm::DirectionalLightComponent* mLightComponent;
    nsm::SkyboxComponent* mSkyboxComponent;
};
