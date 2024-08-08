#include <nsm/gfx/model.h>

#include <nsm/entity/component/cameracomponent.h>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>

nsm::Model::Model(const std::string& path)
    : mPath(path)
    , mObjects()
    , mMeshes()
    , mInstanceIDs()
{
    fastgltf::Parser parser;
    
    fastgltf::Expected<fastgltf::GltfDataBuffer> data = fastgltf::GltfDataBuffer::FromPath(path);
    NSM_ASSERT(data.error() == fastgltf::Error::None, fastgltf::getErrorMessage(data.error()));
    
    fastgltf::Expected<fastgltf::Asset> asset = parser.loadGltf(data.get(), std::filesystem::path{path}.parent_path(), fastgltf::Options::None);
    NSM_ASSERT(asset.error() == fastgltf::Error::None, fastgltf::getErrorMessage(asset.error()));

    NSM_ASSERT(fastgltf::validate(asset.get()) == fastgltf::Error::None, "The glTF 2.0 asset is invalid");

    const fastgltf::Asset& gltf = asset.get();

    for (const fastgltf::Mesh& mesh : gltf.meshes) {
        mMeshes.emplace(mesh.name, new Mesh(gltf, mesh, path));
    }

    for (const fastgltf::Node& node : gltf.nodes) {
        if (node.meshIndex.has_value()) {
            const fastgltf::Mesh& mesh = gltf.meshes[node.meshIndex.value()];
            const std::variant<fastgltf::TRS, fastgltf::math::fmat4x4>& transformOrTRS = node.transform;

            const fastgltf::TRS* trs = std::get_if<fastgltf::TRS>(&transformOrTRS);
            const fastgltf::math::fmat4x4* transform = std::get_if<fastgltf::math::fmat4x4>(&transformOrTRS);

            glm::mat4 transformMatrix = glm::mat4(1.0f);

            if (transform) {
                std::memcpy(&transformMatrix, transform, sizeof(glm::mat4));
            } else {
                fastgltf::math::fmat4x4 trsMatrix(1.0f);
                
                trsMatrix = fastgltf::math::scale(trsMatrix, trs->scale);
                trsMatrix = fastgltf::math::rotate(trsMatrix, trs->rotation);
                trsMatrix = fastgltf::math::translate(trsMatrix, trs->translation);

                std::memcpy(&transformMatrix, &trsMatrix, sizeof(glm::mat4));
            }

            mObjects.emplace(mesh.name, new Object(mMeshes[mesh.name.c_str()], transformMatrix));
        }
    }
}

nsm::Model::~Model() {
    for (auto& [name, mesh] : mMeshes) {
        delete mesh;
    }
}

void nsm::Model::drawOpaque(const RenderInfo& renderInfo) {
    for (auto& [name, object] : mObjects) {
        object->drawOpaque(renderInfo, mInstanceIDs);
    }
}

void nsm::Model::drawTranslucent(const RenderInfo& renderInfo) {
    for (auto& [name, object] : mObjects) {
        object->drawTranslucent(renderInfo, mInstanceIDs);
    }
}

void nsm::Model::addInstance(std::size_t* outID) {
    NSM_ASSERT(outID != nullptr, "Output ID is null");

    *outID = mInstanceIDs.size();
    mInstanceIDs.push_back(outID);

    NSM_ASSERT(mInstanceIDs[*outID] == outID, "Instance ID ", *outID, " not set correctly");

    for (auto& [name, object] : mObjects) {
        object->growInstanceDataBuffer(mInstanceIDs.size());
    }
}

void nsm::Model::removeInstance(std::size_t id) {
    NSM_ASSERT(id < mInstanceIDs.size(), "Instance ID ", id, " out of bounds");
    mInstanceIDs.erase(mInstanceIDs.begin() + id);

    for (std::size_t i = id; i < mInstanceIDs.size(); i++) {
        if (*mInstanceIDs[i] > id) {
            *mInstanceIDs[i] -= 1;
        }
    }

    for (auto& [name, object] : mObjects) {
        object->shrinkInstanceDataBuffer(mInstanceIDs.size(), id);
    }
}

void nsm::Model::setInstanceData(const std::string& objectName, void* data, const std::size_t index) {
    auto it = mObjects.find(objectName);
    if (it == mObjects.end()) {
        NSM_ASSERT(false, "Object ", objectName, " not found in model ", mPath);
    }

    Object& object = *it->second;
    
    object.setInstanceData(data, index);
}

// Mesh

nsm::Model::Mesh::Mesh(const fastgltf::Asset& asset, const fastgltf::Mesh& mesh, const std::string& path) {
    for (const fastgltf::Primitive& primitive : mesh.primitives) {
        const fastgltf::Accessor& position = asset.accessors[primitive.findAttribute("POSITION")->accessorIndex];

        NSM_ASSERT(position.componentType == fastgltf::ComponentType::Float, "Position component type is not float");

        const fastgltf::Attribute* aTexCoord = primitive.findAttribute("TEXCOORD_0");
        const fastgltf::Attribute* aColor = primitive.findAttribute("COLOR_0");
        const fastgltf::Attribute* aNormal = primitive.findAttribute("NORMAL");

        std::vector<Vertex> vertexVector;
        std::vector<u32> indexVector;

        for (std::size_t i = 0; i < position.count; i++) {
            Vertex v;

            v.position = fastgltf::getAccessorElement<glm::vec3>(asset, position, i);

            if (aTexCoord != primitive.attributes.end()) {
                const fastgltf::Accessor& uv = asset.accessors[aTexCoord->accessorIndex];
                v.uv = fastgltf::getAccessorElement<glm::vec2>(asset, uv, i);
            } else {
                v.uv = glm::vec2(0.0f);
            }

            if (aColor != primitive.attributes.end()) {
                const fastgltf::Accessor& color = asset.accessors[aColor->accessorIndex];
                v.color = fastgltf::getAccessorElement<glm::vec3>(asset, color, i);
            } else {
                v.color = glm::vec3(1.0f);
            }

            if (aNormal != primitive.attributes.end()) {
                const fastgltf::Accessor& normal = asset.accessors[aNormal->accessorIndex];
                v.normal = fastgltf::getAccessorElement<glm::vec3>(asset, normal, i);
            } else {
                v.normal = glm::vec3(0.0f);
            }

            vertexVector.push_back(v);
        }

        NSM_ASSERT(primitive.indicesAccessor.has_value(), "Primitive has no indices accessor");

        const fastgltf::Accessor& indices = asset.accessors[primitive.indicesAccessor.value()];

        switch (indices.componentType) {
            case fastgltf::ComponentType::UnsignedByte: {
                for (std::size_t i = 0; i < indices.count; i++) {
                    indexVector.push_back(fastgltf::getAccessorElement<u8>(asset, indices, i));
                }
                break;
            }
            case fastgltf::ComponentType::UnsignedShort: {
                for (std::size_t i = 0; i < indices.count; i++) {
                    indexVector.push_back(fastgltf::getAccessorElement<u16>(asset, indices, i));
                }
                break;
            }
            case fastgltf::ComponentType::UnsignedInt: {
                for (std::size_t i = 0; i < indices.count; i++) {
                    indexVector.push_back(fastgltf::getAccessorElement<u32>(asset, indices, i));
                }
                break;
            }
            default: {
                NSM_ASSERT(false, "Unsupported index component type");
            }
        }

        NSM_ASSERT(primitive.materialIndex.has_value(), "Primitive has no material");

        const fastgltf::Material& material = asset.materials[primitive.materialIndex.value()];
        mShapes.push_back(new Shape(asset, material, path + std::string{material.name.c_str()}, vertexVector, indexVector));
    }
}

nsm::Model::Mesh::~Mesh() {
    for (Shape* shape : mShapes) {
        delete shape;
    }
}

void nsm::Model::Mesh::drawOpaque(const RenderInfo& renderInfo, u32 count, const glm::mat4& localTransform) {    
    for (Shape* shape : mShapes) {
        if (shape->getMaterial()->isTranslucent()) {
            continue;
        }

        shape->draw(renderInfo, count, localTransform);
    }
}

void nsm::Model::Mesh::drawTranslucent(const RenderInfo& renderInfo, u32 count, const glm::mat4& localTransform) {
    for (Shape* shape : mShapes) {
        if (!shape->getMaterial()->isTranslucent()) {
            continue;
        }

        shape->draw(renderInfo, count, localTransform);
    }
}

// Shape

nsm::Model::Mesh::Shape::Shape(const fastgltf::Asset& asset, const fastgltf::Material& material, const std::string& materialIdentifier, const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
    : mMaterial(Material::get(materialIdentifier, asset, material))
    , mVertexArray()
    , mIndexBuffer(indices.data(), indices.size() * sizeof(u32), nsm::BufferUsage::StaticDraw)
    , mVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), sizeof(Vertex), nsm::BufferUsage::StaticDraw)
{
    static const std::array<VertexArray::Attribute, 4> attributes = {
        VertexArray::Attribute{0, 3, VertexArray::DataType::Float, offsetof(Vertex, position), 0, false},
        VertexArray::Attribute{1, 2, VertexArray::DataType::Float, offsetof(Vertex, uv), 0, false},
        VertexArray::Attribute{2, 3, VertexArray::DataType::Float, offsetof(Vertex, color), 0, false},
        VertexArray::Attribute{3, 3, VertexArray::DataType::Float, offsetof(Vertex, normal), 0, false}
    };

    mVertexArray.setLayout(attributes);
    mVertexArray.linkBuffer(mVertexBuffer, 0);
    mVertexArray.linkIndices(mIndexBuffer);
}

void nsm::Model::Mesh::Shape::draw(const RenderInfo& renderInfo, u32 count, const glm::mat4& localTransform) {
    mMaterial->bind();
    mVertexArray.bind();

    mMaterial->getShader()->setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mMaterial->getShader()->setMat4("uNodeTransform", localTransform);

    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer.getCount()), GL_UNSIGNED_INT, nullptr, count);
}

// Object

nsm::Model::Object::Object(Mesh* mesh, const glm::mat4& transform)
    : mMesh(mesh)
    , mTransform(transform)
    , mInstanceDataBuffer(nullptr)
    , mInstanceDataBufferEntrySize(0)
    , mInstanceDataDirty(false)
{ }

nsm::Model::Object::~Object() {
    std::free(mInstanceDataBuffer);
}

void nsm::Model::Object::drawOpaque(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds) {
    if (mInstanceDataDirty) {
        mSSBO.setData(mInstanceDataBufferEntrySize * instanceIds.size(), mInstanceDataBuffer);
        mInstanceDataDirty = false;
    }
    
    mSSBO.bind(0);

    mMesh->drawOpaque(renderInfo, static_cast<u32>(instanceIds.size()), mTransform);
}

void nsm::Model::Object::drawTranslucent(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds) {
    if (mInstanceDataDirty) {
        mSSBO.setData(mInstanceDataBufferEntrySize * instanceIds.size(), mInstanceDataBuffer);
        mInstanceDataDirty = false;
    }
    
    mSSBO.bind(0);

    mMesh->drawTranslucent(renderInfo, static_cast<u32>(instanceIds.size()), mTransform);
}

void nsm::Model::Object::growInstanceDataBuffer(const std::size_t newSize) {
    mInstanceDataDirty = true;

    if (mInstanceDataBuffer == nullptr) {
        mInstanceDataBuffer = std::malloc(mInstanceDataBufferEntrySize * newSize);
        return;
    }

    mInstanceDataBuffer = std::realloc(mInstanceDataBuffer, mInstanceDataBufferEntrySize * newSize);
}

void nsm::Model::Object::shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex) {
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

void nsm::Model::Object::setInstanceData(const void* data, const std::size_t index) {
    NSM_ASSERT(mInstanceDataBuffer != nullptr, "Instance data buffer is null");
    NSM_ASSERT(mInstanceDataBufferEntrySize > 0, "Instance data buffer entry size is 0");

    std::memcpy(static_cast<u8*>(mInstanceDataBuffer) + (mInstanceDataBufferEntrySize * index), data, mInstanceDataBufferEntrySize);
    mInstanceDataDirty = true;
}
