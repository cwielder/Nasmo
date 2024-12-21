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

class IndicatorComponent : public nsm::DrawableComponent {
public:
    IndicatorComponent(glm::vec3 origin, glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f), f32 length = 1.5f, glm::vec4 color1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4 color2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
        : mOrigin(origin)
        , mDirection(direction)
        , mLength(length)
        , mColor1(color1)
        , mColor2(color2)
        , mShaderProgram("shaders/indicatorcube.vsh", "shaders/indicatorcube.fsh")
    { }

    ~IndicatorComponent() override = default;

    void update(glm::vec3 direction, f32 length) {
        mDirection = direction;
        mLength = length;
    }

    void drawOpaque(const nsm::RenderInfo& renderInfo) override {
        mShaderProgram.bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, mOrigin);
        model = glm::scale(model, glm::vec3(0.1f));

        mShaderProgram.setMat4("uModelMtx", model);
        mShaderProgram.setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
        mShaderProgram.setVec4("uColor", mColor1);

        nsm::PrimitiveShape::getCubeVAO().bind();
        nsm::PrimitiveShape::getCubeIBO().draw();
        
        model = glm::translate(model, glm::normalize(mDirection) * mLength);

        mShaderProgram.setMat4("uModelMtx", model);
        mShaderProgram.setVec4("uColor", mColor2);

        nsm::PrimitiveShape::getCubeVAO().bind();
        nsm::PrimitiveShape::getCubeIBO().draw();
    }

    void drawTranslucent(const nsm::RenderInfo& renderInfo) override { }

private:
    glm::vec3 mOrigin;
    glm::vec3 mDirection;
    f32 mLength;
    glm::vec4 mColor1, mColor2;
    nsm::ShaderProgram mShaderProgram;
};

class SunEntity final : public nsm::Entity {
public:
    SunEntity(nsm::Entity::Properties& properties)
        : mIntensity(nsm::JsonHelpers::getFloat(properties, "intensity"))
        , mLightComponent(nullptr)
    { }

    ~SunEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override {
        mLightComponent = new nsm::DirectionalLightComponent(
            0.0f, 0.0f,
            glm::vec3(1.0f, 1.0f, 1.0f),
            mIntensity
        );
        mLightComponent->setTargetLayer("lighting_directional");
        this->addComponent<nsm::DrawableComponent>(mLightComponent);

        mIndicatorComponent = new IndicatorComponent(glm::vec3(5.0f, 0.0f, 5.0f));
        mIndicatorComponent->setTargetLayer("forward");
        this->addComponent<nsm::DrawableComponent>(mIndicatorComponent);

        mSkyboxComponent = new nsm::SkyboxComponent("textures/skybox1/", ".jpg");
        mSkyboxComponent->setTargetLayer("skybox");
        this->addComponent<nsm::DrawableComponent>(mSkyboxComponent);
    }

    void onUpdate(const f32 timeStep) override {
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

        mIndicatorComponent->update(
            mLightComponent->getDirection(),
            indicatorLength
        );
    }

private:
    f32 mIntensity;
    nsm::DirectionalLightComponent* mLightComponent;
    IndicatorComponent* mIndicatorComponent;
    nsm::SkyboxComponent* mSkyboxComponent;
};

NSM_REGISTER_ENTITY(SunEntity);
