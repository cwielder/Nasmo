#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.4f)
            .setLifeSpanVariance(0.05f)
            .setInitialVelocity(glm::vec3(-64.0f, 0.0f, 0.0f))
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/exhaust.png")
        ;
    }
}

class PlayerEntity final : public nsm::Entity {
public:
    PlayerEntity(nsm::Entity::Properties& properties)
        : mTransform(nullptr)
        , mModel(nullptr)
        , mStartPosition(nsm::JsonHelpers::getVec3(properties, "position"))
        , mStartScale(nsm::JsonHelpers::getVec3(properties, "scale"))
    { }

    ~PlayerEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        mTransform = new nsm::TransformComponent();
        mTransform->setPosition(mStartPosition);
        mTransform->setScale(mStartScale);
        this->addComponent<nsm::TransformComponent>(mTransform);

        mModel = new nsm::ModelComponent("models/B2.glb", "main");
        this->addComponent<nsm::DrawableComponent>(mModel);

        mExhaustParticleLeft = new nsm::ParticleComponent();
        mExhaustParticleLeft->setTargetLayer("forward");
        this->addComponent<nsm::DrawableComponent>(mExhaustParticleLeft);
        CreateExhaustParticle(mExhaustParticleLeft);

        mExhaustParticleRight = new nsm::ParticleComponent();
        mExhaustParticleRight->setTargetLayer("forward");
        this->addComponent<nsm::DrawableComponent>(mExhaustParticleRight);
        CreateExhaustParticle(mExhaustParticleRight);

        mModel->setTransformAll(glm::scale(glm::translate(glm::mat4(1.0f), mTransform->getPosition()), mTransform->getScale()));
    }

    void onUpdate(const f64 timeStep) override {
        nsm::ParticleEmitter& emitter = mExhaustParticleLeft->getEmitter();
        
        glm::vec3 scale = mTransform->getScale();
        glm::vec3 position = mTransform->getPosition();
        
        if (ImGui::Begin("Demo Entity")) {
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);
            ImGui::DragFloat3("Position", &position.x, 0.1f);
        } ImGui::End();

        mTransform->setScale(scale);
        mTransform->setPosition(position);

        mExhaustParticleLeft->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, -4.0f));
        mExhaustParticleRight->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, 4.0f));

        mModel->setTransformAll(glm::scale(glm::translate(glm::mat4(1.0f), mTransform->getPosition()), mTransform->getScale()));
    }

private:
    glm::vec3 mStartPosition, mStartScale;
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
};

NSM_REGISTER_ENTITY(PlayerEntity);
