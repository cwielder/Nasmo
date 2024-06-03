#pragma once

#include <nsm/gfx/model.h>
#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/debug/assert.h>

#include <string>
#include <map>

namespace nsm {

    class ModelComponent : public DrawableComponent {
    public:
        ModelComponent(const std::string& path, const std::string& layer, const std::pair<std::string, std::size_t>* meshInstanceDataSizes);
        ~ModelComponent() override;

        void drawOpaque(const RenderInfo& renderInfo) override { NSM_ASSERT(false, "Cannot directly draw from a ModelComponent! Use ModelLayer instead."); }
        void drawTranslucent(const RenderInfo& renderInfo) override { NSM_ASSERT(false, "Cannot directly draw from a ModelComponent! Use ModelLayer instead."); }

        void setInstanceDataDirty(const std::string& meshName);
        void setInstanceData(const std::string& meshName, void* data);

        [[nodiscard]] Model* getModel() const { return mModel; }
        
        static void clearModels();

    private:
        static std::map<std::string, Model*> sModels;

        Model* mModel;
        std::map<std::string /* meshName */, void* /* data */> mInstanceData;
        std::size_t mInstanceID;
    };

}
