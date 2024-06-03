#include <nsm/entity/component/modelcomponent.h>

std::map<std::string, nsm::Model*> nsm::ModelComponent::sModels;

nsm::ModelComponent::ModelComponent(const std::string& path, const std::string& layer, const std::pair<std::string, std::size_t>* meshInstanceDataSizes)
    : mModel(nullptr)
    , mInstanceData()
{
    auto it = sModels.find(path);
    if (it == sModels.end()) {
        sModels[path] = new Model(path);
    }
    mModel = sModels[path];

    for (int i = 0; i < mModel->getMeshCount(); i++) {
        const auto& [meshName, instanceDataSize] = meshInstanceDataSizes[i];
        
        mModel->getMesh(meshName)->setInstanceDataBufferEntrySize(instanceDataSize);
    }
    mModel->addInstance(&mInstanceID);

    this->setTargetLayer(layer);
}

nsm::ModelComponent::~ModelComponent() {
    mModel->removeInstance(mInstanceID);

    if (mModel->getInstanceIDs().empty()) {
        auto it = sModels.find(mModel->getPath());
        delete (*it).second;
        sModels.erase(it);
    }
}

void nsm::ModelComponent::setInstanceDataDirty(const std::string& meshName) {
    mModel->setInstanceData(meshName, mInstanceData[meshName], mInstanceID);
}

void nsm::ModelComponent::setInstanceData(const std::string& meshName, void* data) {
    mInstanceData[meshName] = data;
    mModel->setInstanceData(meshName, data, mInstanceID);
}
