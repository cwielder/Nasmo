#pragma once

#include <nsm/gfx/animatedtexture.h>
#include <nsm/gfx/shaderstorage.h>
#include <nsm/gfx/shader.h>

#include <memory>
#include <vector>

namespace nsm {

    class ParticleEmitter;
    struct RenderInfo;

    class EmitterVisual {
    public:
        void removeSelf(ParticleEmitter* customer) {
            std::erase_if(emitters, [customer](ParticleEmitter* emitter) {
                return emitter == customer;
            });
        }

    private:
        friend class ParticleRenderer;

        bool depth;
        std::unique_ptr<AnimatedTexture> texture;
        std::string texturePath;
        std::vector<ParticleEmitter*> emitters;
        ShaderStorage storage;
    };

    class ParticleRenderer {
    public:
        struct GPUParticle {
            glm::vec3 size;
            float padding;
            glm::vec3 position;
            float lifeTime;
        };

    public:
        ParticleRenderer();
        ~ParticleRenderer() = default;

        const std::shared_ptr<EmitterVisual> getOrCreateEmitterVisual(ParticleEmitter* customer, const std::string& path, const bool depth);

        void render(const RenderInfo& renderInfo);

    private:
        std::vector<std::shared_ptr<EmitterVisual>> mEmitterVisuals;
        ShaderProgram mShader;
    };

}
