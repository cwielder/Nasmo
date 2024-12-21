#pragma once

#include <nsm/common.h>
#include <nsm/util/randomsource.h>
#include <nsm/gfx/shader.h>

#include <glm/glm.hpp>

#include <vector>

namespace nsm {

    struct RenderInfo;

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter() = default;

        ParticleEmitter& setPosition(const glm::vec3& position) {
            mPosition = position;
            return *this;
        }

        glm::vec3 getPosition() const {
            return mPosition;
        }

        ParticleEmitter& setEmitRadius(const f32 emitRadius) {
            mEmitRadius = emitRadius;
            return *this;
        }

        f32 getEmitRadius() const {
            return mEmitRadius;
        }

        ParticleEmitter& setEmitRate(const f32 emitRate) {
            mEmitRate = emitRate;
            return *this;
        }

        f32 getEmitRate() const {
            return mEmitRate;
        }

        ParticleEmitter& setInitialVelocity(const glm::vec3& initialVelocity) {
            mInitialVelocity = initialVelocity;
            return *this;
        }

        glm::vec3 getInitialVelocity() const {
            return mInitialVelocity;
        }

        ParticleEmitter& setLifeSpan(const f32 lifeSpan) {
            mLifeSpan = lifeSpan;
            return *this;
        }

        f32 getLifeSpan() const {
            return mLifeSpan;
        }

        ParticleEmitter& setAcceleration(const glm::vec3& acceleration) {
            mAcceleration = acceleration;
            return *this;
        }

        glm::vec3 getAcceleration() const {
            return mAcceleration;
        }

        ParticleEmitter& setLocalSpace(const bool localSpace) {
            mLocalSpace = localSpace;
            return *this;
        }

        bool getLocalSpace() const {
            return mLocalSpace;
        }

        void update(const f32 timeStep);
        void render(const RenderInfo& renderInfo);

    private:
        static inline ShaderProgram* sShader = nullptr;

        /*
            Per-particle data:
            motion:
            - world-position or offset from emitter position
            - velocity
            - current lifetime (0.0f to 1.0f)
            visual:
        */

        /*
            Asterisk denotes "supports variance"
            ParticleEmitter data:
            emission:
            - position
            - world-space or local-space
            - emit radius
            - emit volume
            - emit rate
            creation params:
            - initial velocity*
            - life span*
            - acceleration*
        */

        struct Particle {
            union {
                glm::vec3 position;
                glm::vec3 offset;
            };
            glm::vec3 velocity;
            f32 lifeTime;
        };

        std::vector<Particle> mParticles;
        RandomSource mRandom;
        
        glm::vec3 mPosition;
        f32 mEmitRadius; // TODO: Support cubic bounding volume
        f32 mEmitRate;

        glm::vec3 mInitialVelocity;
        f32 mLifeSpan;
        glm::vec3 mAcceleration;
        bool mLocalSpace = false;
    };

}
