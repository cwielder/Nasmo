#include <nsm/gfx/model.h>

#include <nsm/entity/component/cameracomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <simdjson.h>

nsm::Model::Model(const std::string& path)
    : mPath(path)
    , mMeshes()
    , mInstanceIDs()
{
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document doc = parser.iterate(json);

    auto vertices = doc["vertices"].get_array();

    std::vector<Vertex> vertexVector;

    for (auto vertex : vertices) {
        Vertex v;

        auto vertexObject = vertex.get_object();

        const glm::vec3 position = JsonHelpers::getVec3(vertexObject, "position");
        v.posX = position.x;
        v.posY = position.y;
        v.posZ = position.z;

        const glm::vec2 texCoord = JsonHelpers::getVec2(vertexObject, "texcoord");
        v.texU = texCoord.x;
        v.texV = texCoord.y;

        const glm::vec3 color = JsonHelpers::getVec3(vertexObject, "color");
        v.colR = color.r;
        v.colG = color.g;
        v.colB = color.b;

        const glm::vec3 normal = JsonHelpers::getVec3(vertexObject, "normal");
        v.nmlX = normal.x;
        v.nmlY = normal.y;
        v.nmlZ = normal.z;

        vertexVector.push_back(v);
    }

    mVertexBuffer.init(vertexVector.data(), vertexVector.size(), sizeof(Vertex), nsm::BufferUsage::StaticDraw);
    mVertexBuffer.markAttribute(0, 3, VertexBuffer::DataType::Float, 0 * sizeof(f32), false);
    mVertexBuffer.markAttribute(1, 2, VertexBuffer::DataType::Float, 3 * sizeof(f32), false);
    mVertexBuffer.markAttribute(2, 3, VertexBuffer::DataType::Float, 5 * sizeof(f32), false);
    mVertexBuffer.markAttribute(3, 3, VertexBuffer::DataType::Float, 8 * sizeof(f32), false);

    auto meshes = doc["meshes"].get_array();

    for (auto m : meshes) {
        auto meshObject = m.get_object();

        std::vector<u32> indices;
        auto indicesArray = meshObject["indices"].get_array();
        indices.reserve(indicesArray.count_elements());

        for (auto index : indicesArray) {
            indices.push_back(static_cast<u32>(index.get_uint64().value()));
        }

        mMeshes.emplace(meshObject["name"].get_string().value(), new Mesh(
            meshObject["material"].get_string().value(),
            indices,
            mVertexBuffer
        ));
    }
}

nsm::Model::~Model() {
    for (auto& [name, mesh] : mMeshes) {
        delete mesh;
    }
}

void nsm::Model::drawOpaque(const RenderInfo& renderInfo) {
    for (auto& [name, mesh] : mMeshes) {
        if (mesh->getMaterial()->isTranslucent()) {
            continue;
        }

        mesh->draw(renderInfo, mInstanceIDs);
    }
}

void nsm::Model::drawTranslucent(const RenderInfo& renderInfo) {
    for (auto& [name, mesh] : mMeshes) {
        if (!mesh->getMaterial()->isTranslucent()) {
            continue;
        }

        mesh->draw(renderInfo, mInstanceIDs);
    }
}

void nsm::Model::addInstance(std::size_t* outID) {
    *outID = mInstanceIDs.size();
    mInstanceIDs.push_back(outID);

    for (auto& [name, mesh] : mMeshes) {
        mesh->growInstanceDataBuffer(mInstanceIDs.size());
    }
}

void nsm::Model::removeInstance(std::size_t id) {
    mInstanceIDs.erase(mInstanceIDs.begin() + id);

    for (std::size_t i = id; i < mInstanceIDs.size(); i++) {
        if (*mInstanceIDs[i] > id) {
            mInstanceIDs[i]--;
        }
    }

    for (auto& [name, mesh] : mMeshes) {
        mesh->shrinkInstanceDataBuffer(mInstanceIDs.size(), id);
    }
}

void nsm::Model::setInstanceData(const std::string& meshName, void* data, const std::size_t index) {
    auto it = mMeshes.find(meshName);
    if (it == mMeshes.end()) {
        NSM_ASSERT(false, "Mesh ", meshName, " not found in model ", mPath);
    }

    Mesh& mesh = *it->second;
    
    mesh.setInstanceData(data, index);
}

// Mesh

nsm::Model::Mesh::Mesh(std::string_view material, const std::vector<u32>& indices, const VertexBuffer& vertexBuffer)
    : mMaterial(Material::get(std::string{material}))
    , mVertexArray()
    , mIndexBuffer(indices.data(), indices.size(), nsm::BufferUsage::StaticDraw)
    , mInstanceDataBuffer(nullptr)
    , mInstanceDataBufferEntrySize(0)
    , mInstanceDataDirty(false)
    , mSSBO()
{
    mVertexArray.linkBuffer(vertexBuffer);
    mVertexArray.linkIndices(mIndexBuffer);
}

nsm::Model::Mesh::~Mesh() {
    std::free(mInstanceDataBuffer);
}

void nsm::Model::Mesh::draw(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds) {
    if (mInstanceDataDirty) {
        mSSBO.setData(mInstanceDataBufferEntrySize * instanceIds.size(), mInstanceDataBuffer);
        mInstanceDataDirty = false;
    }
    
    mMaterial->bind();
    mVertexArray.bind();
    mIndexBuffer.bind();
    mSSBO.bind(0);

    mMaterial->getShader()->setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer.getCount()), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(instanceIds.size()));
}

void nsm::Model::Mesh::growInstanceDataBuffer(const std::size_t newSize) {
    mInstanceDataDirty = true;

    if (mInstanceDataBuffer == nullptr) {
        mInstanceDataBuffer = std::malloc(mInstanceDataBufferEntrySize * newSize);
        return;
    }

    mInstanceDataBuffer = std::realloc(mInstanceDataBuffer, mInstanceDataBufferEntrySize * newSize);
}

void nsm::Model::Mesh::shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex) {
    u8* newBuffer = static_cast<u8*>(std::malloc(mInstanceDataBufferEntrySize * newSize));

    // Copy the data before the removed instance
    std::memcpy(newBuffer, mInstanceDataBuffer, mInstanceDataBufferEntrySize * missingIndex);

    // Copy the data after the removed instance (if any) but skip the removed instance
    std::memcpy(newBuffer + mInstanceDataBufferEntrySize * missingIndex, static_cast<u8*>(mInstanceDataBuffer) + mInstanceDataBufferEntrySize * (missingIndex + 1), mInstanceDataBufferEntrySize * (newSize - missingIndex));

    // Free the old buffer
    std::free(mInstanceDataBuffer);

    mInstanceDataBuffer = newBuffer;
    mInstanceDataDirty = true;
}

void nsm::Model::Mesh::setInstanceData(const void* data, const std::size_t index) {
    NSM_ASSERT(mInstanceDataBuffer != nullptr, "Instance data buffer is null");
    NSM_ASSERT(mInstanceDataBufferEntrySize > 0, "Instance data buffer entry size is 0");

    std::memcpy(static_cast<u8*>(mInstanceDataBuffer) + (mInstanceDataBufferEntrySize * index), data, mInstanceDataBufferEntrySize);
    mInstanceDataDirty = true;
}
