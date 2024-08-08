#include <nsm/entity/entity.h>
#include <nsm/entity/component/lightcomponent.h>

#include <imgui.h>

class LightEntity final : public nsm::Entity {
public:
    LightEntity(nsm::Entity::Properties&)
        : mLightComponent(nullptr)
    { }

    ~LightEntity() override = default;

    void onCreate() override {
        mLightComponent = new nsm::PointLightComponent(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            10.0f
        );
        mLightComponent->setTargetLayer("lighting");
        this->addComponent<nsm::DrawableComponent>(mLightComponent);
    }

    void onUpdate(const f32 timeStep) override {
        static f32 posX = 0.0f, posY = 0.0f, posZ = 0.0f;
        static f32 colorR = 1.0f, colorG = 1.0f, colorB = 1.0f;
        static f32 intensity = 1.0f;
        static f32 radius = 10.0f;

        if (ImGui::Begin("Light")) {
            ImGui::SliderFloat("Position X", &posX, -50.0f, 50.0f);
            ImGui::SliderFloat("Position Y", &posY, -50.0f, 50.0f);
            ImGui::SliderFloat("Position Z", &posZ, -50.0f, 50.0f);
            ImGui::SliderFloat("Color R", &colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &colorB, 0.0f, 1.0f);
            ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f);
            ImGui::SliderFloat("Radius", &radius, 0.0f, 50.0f);
        } ImGui::End();

        mLightComponent->setPosition(glm::vec3(posX, posY, posZ));
        mLightComponent->setColor(glm::vec3(colorR, colorG, colorB));
        mLightComponent->setIntensity(intensity);
        mLightComponent->setRadius(radius);
    }

private:
    nsm::PointLightComponent* mLightComponent;
};

NSM_REGISTER_ENTITY(LightEntity);
