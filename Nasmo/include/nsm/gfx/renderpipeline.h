#pragma once

#include <glm/vec2.hpp>

#include <vector>
#include <string>

namespace nsm {

    class Layer;
    class Framebuffer;

    class RenderPipeline {
    public:
        RenderPipeline() = default;
        virtual ~RenderPipeline();

        virtual void render(const Framebuffer* framebuffer) = 0;

    protected:
        template<typename T>
        T* pushLayer(const std::string& name) requires std::is_base_of_v<Layer, T> {
            T* layer = new T(name);
            mLayers.emplace_back(std::make_pair(std::hash<std::string>{}(name), layer));
            
            return layer;
        }

    private:
        using LayerContainer = std::vector<std::pair<std::size_t /* nameHash */ , Layer*>>;

        friend class Renderer;
        Layer* getLayer(const std::size_t hash);
        LayerContainer& getLayers() { return mLayers; }
        void resize(const glm::u32vec2& size);

        LayerContainer mLayers;
    };

}
