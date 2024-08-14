#include <nsm/gfx/renderpipeline.h>

#include <nsm/gfx/layer/layer.h>
#include <nsm/debug/assert.h>

nsm::RenderPipeline::~RenderPipeline() {
    for (auto& [hash, layer] : mLayers) {
        delete layer;
    }
}

nsm::Layer* nsm::RenderPipeline::getLayer(const std::size_t hash) {
    auto it = std::find_if(mLayers.begin(), mLayers.end(), [hash](const auto& pair) {
        return pair.first == hash;
    });

    NSM_ASSERT(it != mLayers.end(), "Unable to find layer with hash: ", hash);

    return it->second;
}

void nsm::RenderPipeline::resize(const glm::u32vec2& size) {    
    for (auto& [hash, layer] : mLayers) {
        layer->resize(size);
    }

    this->onResize(size);
}
