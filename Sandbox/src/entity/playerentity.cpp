#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/inputcomponent.h>
#include <nsm/entity/component/lightcomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.21f)
            .setLifeSpanVariance(0.2f)
            .setInitialVelocity(glm::vec3(-64.0f, 0.0f, 0.0f))
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/exhaust.png")
            .setDepth(false)
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

        mExhaustLightLeft = new nsm::PointLightComponent(mTransform->getPosition() + glm::vec3(-7.0f, -0.5f, -4.0f), glm::vec3(1.0f, 0.5f, 4.0f), 1.0f);
        mExhaustLightLeft->setTargetLayer("lighting_point");
        this->addComponent<nsm::DrawableComponent>(mExhaustLightLeft);

        mExhaustLightRight = new nsm::PointLightComponent(mTransform->getPosition() + glm::vec3(-7.0f, -0.5f, 4.0f), glm::vec3(1.0f, 0.5f, 4.0f), 1.0f);
        mExhaustLightRight->setTargetLayer("lighting_point");
        this->addComponent<nsm::DrawableComponent>(mExhaustLightRight);

        mInput = new nsm::InputComponent();
        this->addComponent<nsm::InputComponent>(mInput);
        mInput->setOnKeyPress([this](const nsm::KeyPressEvent* event) {
            switch (event->getKey()) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT: {
                    mLeftPressed = true;
                    break;
                }

                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT: {
                    mRightPressed = true;
                    break;
                }
            }
        });

        mInput->setOnKeyRelease([this](const nsm::KeyReleaseEvent* event) {
            switch (event->getKey()) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT: {
                    mLeftPressed = false;
                    break;
                }

                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT: {
                    mRightPressed = false;
                    break;
                }
            }
        });

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, mTransform->getPosition());
        mtx = glm::rotate(mtx, glm::radians(mVelocity), glm::vec3(1.0f, 1.0f, 0.0f));
        mtx = glm::scale(mtx, mTransform->getScale());

        mModel->setTransformAll(mtx);
    }

    void onUpdate(const f64 timeStep) override {
        nsm::ParticleEmitter& emitter = mExhaustParticleLeft->getEmitter();

        static constexpr f32 cMaxVelocity = 0.06f;
        static constexpr f32 cAcceleration = 0.25f;
        static constexpr f32 cFriction = 0.2f;

        if (mLeftPressed) {
            mAcceleration = -cAcceleration;
        } else if (mRightPressed) {
            mAcceleration = cAcceleration;
        } else {
            mAcceleration = 0.0f;
        }

        mVelocity += mAcceleration * timeStep;
        mVelocity = glm::clamp(mVelocity, -cMaxVelocity, cMaxVelocity);

        // Friction
        if (mVelocity > 0.0f) {
            mVelocity -= cFriction * timeStep;
            mVelocity = glm::max(mVelocity, 0.0f);
        } else if (mVelocity < 0.0f) {
            mVelocity += cFriction * timeStep;
            mVelocity = glm::min(mVelocity, 0.0f);
        }

        glm::vec3 position = mTransform->getPosition();
        position += glm::vec3(0.0f, 0.0f, mVelocity);

        mTransform->setPosition(position);

        mExhaustParticleLeft->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, -4.0f));
        mExhaustParticleRight->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, 4.0f));

        mExhaustLightLeft->setPosition(position + glm::vec3(-8.1f, -0.5f, -3.5f));
        mExhaustLightRight->setPosition(position + glm::vec3(-8.1f, -0.5f, 5.5f));

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, mTransform->getPosition());
        mtx = glm::rotate(mtx, glm::radians(-mVelocity * 400.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        mtx = glm::scale(mtx, mTransform->getScale());

        mModel->setTransformAll(mtx);
    }

private:
    glm::vec3 mStartPosition, mStartScale;
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;
    nsm::LightComponent* mExhaustLightLeft;
    nsm::LightComponent* mExhaustLightRight;

    bool mLeftPressed = false, mRightPressed = false;
    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
};

NSM_REGISTER_ENTITY(PlayerEntity);
