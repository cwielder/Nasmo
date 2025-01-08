#pragma once

#include <nsm/gfx/Model.h>
#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/debug/Assert.h>

#include <string>
#include <map>
#include <unordered_map>

namespace nsm {

    class ModelComponent : public DrawableComponent {
    public:
        ModelComponent(const std::string& path, const std::string& layer);
        ModelComponent(const std::string& path, const std::string& layer, const std::unordered_map<std::string, std::size_t>& meshInstanceDataSizes);
        ~ModelComponent() override;

        void drawOpaque(const RenderInfo& renderInfo) override { NSM_ASSERT(false, "Cannot directly draw from a ModelComponent! Use ModelLayer instead."); }
        void drawTranslucent(const RenderInfo& renderInfo) override { NSM_ASSERT(false, "Cannot directly draw from a ModelComponent! Use ModelLayer instead."); }

        void setInstanceDataDirty(const std::string& objectName);
        void setInstanceData(const std::string& objectName, void* data);

        void setTransformAll(const glm::mat4& transform);
        void setTransform(const std::string& objectName, const glm::mat4& transform);

        [[nodiscard]] Model* getModel() const { return mModel; }
        
        static void clearModels();

    private:
        static std::map<std::string, Model*> sModels;

        Model* mModel;
        std::map<std::string /* objectName */, void* /* data */> mInstanceData;
        std::size_t mInstanceID;
    };

}
