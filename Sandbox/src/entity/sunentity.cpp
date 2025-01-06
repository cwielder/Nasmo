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

private:
    f32 mIntensity, mYaw, mPitch;
    nsm::DirectionalLightComponent* mLightComponent;
    nsm::SkyboxComponent* mSkyboxComponent;
};

NSM_REGISTER_ENTITY(SunEntity);
