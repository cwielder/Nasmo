#include <nsm/entity/entity.h>
#include <nsm/entity/component/lightcomponent.h>
#include <nsm/util/jsonhelpers.h>
#include <imgui.h>

class LightEntity final : public nsm::Entity {
public:
    LightEntity(nsm::Entity::Properties& properties)
        : mPosition(nsm::JsonHelpers::getVec3(properties, "position"))
        , mIntensity(nsm::JsonHelpers::getFloat(properties, "intensity"))
        , mLightComponent(nullptr)
    { }

    ~LightEntity() override = default;

    void onCreate() override {
        mLightComponent = new nsm::PointLightComponent(
            mPosition,
            glm::vec3(1.0f, 1.0f, 1.0f),
            mIntensity
        );
        mLightComponent->setTargetLayer("lighting_point");
        this->addComponent<nsm::DrawableComponent>(mLightComponent);
    }

    void onUpdate(const f32 timeStep) override {
        static f32 posX = mPosition.x, posY = mPosition.y, posZ = mPosition.z;
        static f32 colorR = 1.0f, colorG = 1.0f, colorB = 1.0f;
        static f32 intensity = mIntensity;

        if (ImGui::Begin("Light")) {
            ImGui::SliderFloat("Position X", &posX, -50.0f, 50.0f);
            ImGui::SliderFloat("Position Y", &posY, -50.0f, 50.0f);
            ImGui::SliderFloat("Position Z", &posZ, -50.0f, 50.0f);
            ImGui::SliderFloat("Color R", &colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &colorB, 0.0f, 1.0f);
            ImGui::DragFloat("Intensity", &intensity, 0.1f);
        } ImGui::End();

        mLightComponent->setPosition(glm::vec3(posX, posY, posZ));
        mLightComponent->setColor(glm::vec3(colorR, colorG, colorB));
        mLightComponent->setIntensity(intensity);
    }

private:
    glm::vec3 mPosition;
    f32 mIntensity;
    nsm::PointLightComponent* mLightComponent;
};

NSM_REGISTER_ENTITY(LightEntity);
