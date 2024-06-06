#pragma once

#include <nsm/gfx/shader.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/gfx/graphicscontext.h>
#include <nsm/gfx/renderinfo.h>

#include <glm/vec2.hpp>

#include <type_traits>
#include <string>

namespace nsm {

    class Layer;
    class DrawableComponent;

    class LayerStack {
    public:
        LayerStack(const glm::u32vec2 &size);
        ~LayerStack();

        template<typename T>
        T* pushLayer(const std::string& name) requires std::is_base_of_v<Layer, T> {
            T* layer = new T(name);
            mLayers.emplace_back(std::make_pair(std::hash<std::string>{}(name), layer));
            
            return layer;
        }

        void popLayer();
        void removeLayer(const std::string& name);
        void clearLayers();

        [[nodiscard]] Layer* getLayer(const std::size_t hash);
        [[nodiscard]] Layer* getLayer(const std::string& name) { return this->getLayer(std::hash<std::string>{}(name)); }

        template <typename T>
        T* getLayer(const std::string& name) requires std::is_base_of_v<Layer, T> {
            return static_cast<T*>(this->getLayer(name));
        }

        void resize(const glm::u32vec2& size);

        void pushDrawable(DrawableComponent* drawable);

        void drawLayers() const;

    private:
        using LayerContainer = std::vector<std::pair<std::size_t, Layer*>>; // TODO: Use a map instead of a vector for faster lookup?

        LayerContainer::iterator getLayerIterator(const std::size_t hash);

        LayerContainer mLayers;
        Framebuffer mFramebuffer;
        ShaderProgram mCompositorShader;
        GraphicsContext mGraphicsContext;
    };

}
