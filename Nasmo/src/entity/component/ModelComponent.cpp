#include <nsm/entity/component/ModelComponent.h>

std::map<std::string, nsm::Model*> nsm::ModelComponent::sModels;

nsm::ModelComponent::ModelComponent(const std::string& path, const std::string& layer)
    : mModel(nullptr)
    , mInstanceData()
{
    auto it = sModels.find(path);
    if (it == sModels.end()) {
        sModels[path] = new Model(path);
    }
    mModel = sModels[path];

    mModel->addInstance(&mInstanceID);

    this->setTargetLayer(layer);
}

nsm::ModelComponent::ModelComponent(const std::string& path, const std::string& layer, const std::unordered_map<std::string, std::size_t>& meshInstanceDataSizes)
    : mModel(nullptr)
    , mInstanceData()
{
    auto it = sModels.find(path);
    if (it == sModels.end()) {
        sModels[path] = new Model(path);
    }
    mModel = sModels[path];

    for (const auto& [objectName, instanceDataSize] : meshInstanceDataSizes) {
        Model::Object* object = mModel->getObject(objectName);
        NSM_ASSERT(object, "Object not found: ", objectName);
        NSM_ASSERT(!object->isTransformOnly(), "Object is transform only: ", objectName);

        static_cast<Model::MeshObject*>(object)->setInstanceDataBufferEntrySize(instanceDataSize);
    }

    mModel->addInstance(&mInstanceID);

    this->setTargetLayer(layer);
}

nsm::ModelComponent::~ModelComponent() {
    mModel->removeInstance(mInstanceID);
}

void nsm::ModelComponent::setInstanceDataDirty(const std::string& meshName) {
    mModel->setInstanceData(meshName, mInstanceData[meshName], mInstanceID);
}

void nsm::ModelComponent::setInstanceData(const std::string& meshName, void* data) {
    mInstanceData[meshName] = data;
    mModel->setInstanceData(meshName, data, mInstanceID);
}

void nsm::ModelComponent::setTransformAll(const glm::mat4& transform) {
    mModel->setTransformAll(mInstanceID, transform);
}

void nsm::ModelComponent::setTransform(const std::string& objectName, const glm::mat4& transform) {
    mModel->setTransform(mInstanceID, objectName, transform);
}

void nsm::ModelComponent::clearModels() {
    for (auto& [path, model] : sModels) {
        delete model;
    }
    sModels.clear();
}
