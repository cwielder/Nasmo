#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

class PlaneModel {
public:
    PlaneModel(nsm::Entity* owner)
        : mModelComponent(new nsm::ModelComponent("models/plane.glb", "main"))
    {
        owner->addComponent<nsm::DrawableComponent>(mModelComponent);
    }

    void setTrans(const glm::vec3& scale, const glm::vec3& pos) {
        glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), pos), scale);
        mModelComponent->setTransformAll(transform);
    }

private:
    nsm::ModelComponent* mModelComponent;
};

class DemoEntity final : public nsm::Entity {
public:
    DemoEntity(nsm::Entity::Properties& properties)
        : mTransform(nullptr)
        , mModel(this)
        , mStartPosition(nsm::JsonHelpers::getVec3(properties, "position"))
    { }

    ~DemoEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        mTransform = new nsm::TransformComponent();
        mTransform->setPosition(mStartPosition);
        this->addComponent<nsm::TransformComponent>(mTransform);

        mParticle = new nsm::ParticleComponent();
        mParticle->setTargetLayer("forward");
        this->addComponent<nsm::DrawableComponent>(mParticle);

        mParticle->getEmitter()
            .setEmitRadius(3.2f)
            .setEmitRate(24.0f)
            .setLifeSpan(1.5f)
            .setLifeSpanVariance(0.5f)
            .setInitialVelocity(glm::vec3(0.0f, 5.0f, 2.0f))
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/animatlas.png", 2.0f)
        ;

        mModel.setTrans(mTransform->getScale(), mTransform->getPosition());
    }

    void onUpdate(const f64 timeStep) override {
        nsm::ParticleEmitter& emitter = mParticle->getEmitter();
        
        glm::vec3 scale = mTransform->getScale();
        glm::vec3 position = mTransform->getPosition();

        f32 emitRadius = emitter.getEmitRadius(), emitRate = emitter.getEmitRate(), lifeSpan = emitter.getLifeSpan();
        glm::vec3 initialVelocity = emitter.getInitialVelocity(), acceleration = emitter.getAcceleration();
        glm::vec3 initialVelocityVariance = emitter.getInitialVelocityVariance(), accelerationVariance = emitter.getAccelerationVariance();
        f32 lifeSpanVariance = emitter.getLifeSpanVariance();
        bool localSpace = emitter.getLocalSpace();
        u32 particleLimit = emitter.getParticleLimit();
        glm::vec3 startSize = emitter.getStartSize(), startSizeVariance = emitter.getStartSizeVariance();
        glm::vec3 endSize = emitter.getEndSize(), endSizeVariance = emitter.getEndSizeVariance();
        
        if (ImGui::Begin("Demo Entity")) {
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);
            ImGui::DragFloat3("Position", &position.x, 0.1f);
        } ImGui::End();

        if (ImGui::Begin("Emission")) {
            ImGui::DragFloat("Emit Radius", &emitRadius, 0.1f);
            ImGui::DragFloat("Emit Rate", &emitRate, 0.1f);
            ImGui::DragFloat("Life Span", &lifeSpan, 0.1f);
            ImGui::PushID("Life Span Variance");
            ImGui::DragFloat("Variance", &lifeSpanVariance, 0.1f);
            ImGui::PopID();
            ImGui::DragInt("Particle Limit", reinterpret_cast<int*>(&particleLimit), 1);
            ImGui::Checkbox("Local Space", &localSpace);
        } ImGui::End();

        if (ImGui::Begin("Motion")) {
            ImGui::DragFloat3("Initial Velocity", &initialVelocity.x, 0.1f);
            ImGui::PushID("Initial Velocity Variance");
            ImGui::DragFloat3("Variance", &initialVelocityVariance.x, 0.1f);
            ImGui::PopID();

            ImGui::DragFloat3("Acceleration", &acceleration.x, 0.1f);
            ImGui::PushID("Acceleration Variance");
            ImGui::DragFloat3("Variance", &accelerationVariance.x, 0.1f);
            ImGui::PopID();

            ImGui::Separator();

            ImGui::DragFloat3("Start Size", &startSize.x, 0.1f);
            ImGui::PushID("Start Size Variance");
            ImGui::DragFloat3("Variance", &startSizeVariance.x, 0.1f);
            ImGui::PopID();

            ImGui::DragFloat3("End Size", &endSize.x, 0.1f);
            ImGui::PushID("End Size Variance");
            ImGui::DragFloat3("Variance", &endSizeVariance.x, 0.1f);
            ImGui::PopID();
        } ImGui::End();

        mTransform->setScale(scale);
        mTransform->setPosition(position);

        mParticle->getEmitter()
            .setPosition(position)
            .setEmitRadius(emitRadius)
            .setEmitRate(emitRate)
            .setLifeSpan(lifeSpan)
            .setInitialVelocity(initialVelocity)
            .setAcceleration(acceleration)
            .setLocalSpace(localSpace)
            .setParticleLimit(particleLimit)
            .setStartSize(startSize)
            .setStartSizeVariance(startSizeVariance)
            .setEndSize(endSize)
            .setEndSizeVariance(endSizeVariance)
            .setInitialVelocityVariance(initialVelocityVariance)
            .setLifeSpanVariance(lifeSpanVariance)
            .setAccelerationVariance(accelerationVariance)
        ;

        mModel.setTrans(scale, position);
    }

private:
    glm::vec3 mStartPosition;
    nsm::TransformComponent* mTransform;
    PlaneModel mModel;
    nsm::ParticleComponent* mParticle;
};

NSM_REGISTER_ENTITY(DemoEntity);
