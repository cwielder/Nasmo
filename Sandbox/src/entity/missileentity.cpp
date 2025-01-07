#include <nsm/entity/entity.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/lightcomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateMissileExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.151f)
            .setLifeSpanVariance(0.15f)
            .setInitialVelocity(glm::vec3(-64.0f, 0.0f, 0.0f))
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setVisual("textures/exhaust_missile.png", false)
        ;
    }
}

class MissileEntity final : public nsm::Entity {
public:
    MissileEntity(nsm::Entity::Properties& properties)
        : mTransform(nullptr)
        , mModel(nullptr)
    { }

    ~MissileEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        mLifeTime = 5.0f;

        mInertia = nsm::JsonHelpers::getFloat(properties, "velocity");
        mAcceleration = glm::vec3(12.0f, -9.81f, 0.0f);
        mVelocity = glm::vec3(10.0f, 0.0f, mInertia * 50.0f);

        mTransform = new nsm::TransformComponent();
        mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
        mTransform->setScale(glm::vec3(5.0f));
        this->addComponent<nsm::TransformComponent>(mTransform);

        mExhaustParticle = new nsm::ParticleComponent();
        mExhaustParticle->setTargetLayer("particles");
        this->addComponent<nsm::DrawableComponent>(mExhaustParticle);
        CreateMissileExhaustParticle(mExhaustParticle);

        mModel = new nsm::ModelComponent("models/missile.glb", "main");
        this->addComponent<nsm::DrawableComponent>(mModel);

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, mTransform->getPosition());
        mtx = glm::rotate(mtx, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mtx = glm::scale(mtx, mTransform->getScale());

        mModel->setTransformAll(mtx);
    }

    void onUpdate(const f64 timeStep) override {
        mLifeTime -= static_cast<f32>(timeStep);
        if (mLifeTime <= 0.0f) {
            this->setAlive(false);
            return;
        }

        mVelocity += mAcceleration * glm::vec3(timeStep);
        mVelocity = glm::clamp(mVelocity, -100.0f, 100.0f);

        glm::vec3 position = mTransform->getPosition();
        position += mVelocity * glm::vec3(static_cast<f32>(timeStep * 10.0f));
        mTransform->setPosition(position);

        mExhaustParticle->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, 0.0f));

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, mTransform->getPosition());
        mtx = glm::rotate(mtx, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mtx = glm::scale(mtx, mTransform->getScale());

        mModel->setTransformAll(mtx);
    }

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticle;

    f32 mInertia;
    glm::vec3 mAcceleration;
    glm::vec3 mVelocity;

    f32 mLifeTime;
};

NSM_REGISTER_ENTITY(MissileEntity);
