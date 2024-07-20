#pragma once

#include <nsm/common.h>
#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/material.h>
#include <nsm/gfx/shaderstorage.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/debug/assert.h>

#include <map>
#include <string>

namespace fastgltf {
    class Asset;
    struct Mesh;
    struct Material;
}

namespace nsm {

    class Model final {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec2 uv;
            glm::vec3 color;
            glm::vec3 normal;
        };

    public:
        class Mesh final {
            NSM_NO_COPY(Mesh);

        public:
            class Shape final {
            public:
                Shape(const fastgltf::Asset& asset, const fastgltf::Material& material, const std::string& materialIdentifier, const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
                ~Shape() = default;

                void draw(const RenderInfo& renderInfo, const u32 count, const glm::mat4& localTransform);

                [[nodiscard]] const Material* getMaterial() const { return mMaterial; }

            private:
                Material* mMaterial;
                VertexArray mVertexArray;
                IndexBuffer mIndexBuffer;
                VertexBuffer mVertexBuffer;
            };

        public:
            Mesh(const fastgltf::Asset& asset, const fastgltf::Mesh& mesh, const std::string& path);
            ~Mesh();

            void drawOpaque(const RenderInfo& renderInfo, const u32 count, const glm::mat4& localTransform);
            void drawTranslucent(const RenderInfo& renderInfo, const u32 count, const glm::mat4& localTransform);

        private:
            std::vector<Shape*> mShapes;
        };

        class Object final {
        public:
            Object(Mesh* mesh, const glm::mat4& transform);
            ~Object();

            void growInstanceDataBuffer(const std::size_t newSize);
            void shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex);
            void setInstanceData(const void* data, const std::size_t index);
            void setInstanceDataBufferEntrySize(const std::size_t entrySize) { mInstanceDataBufferEntrySize = entrySize; }

            void drawOpaque(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);
            void drawTranslucent(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);

        private:
            Mesh* mMesh;
            glm::mat4 mTransform;
            void* mInstanceDataBuffer;
            std::size_t mInstanceDataBufferEntrySize;
            bool mInstanceDataDirty;
            ShaderStorage mSSBO;
        };

    public:
        Model(const std::string& path);
        ~Model();

        void drawOpaque(const RenderInfo& renderInfo);
        void drawTranslucent(const RenderInfo& renderInfo);

        void addInstance(std::size_t* outID);
        void removeInstance(std::size_t id);

        void setInstanceData(const std::string& objectName, void* data, const std::size_t index);

        [[nodiscard]] const std::vector<std::size_t*>& getInstanceIDs() { return mInstanceIDs; }
        [[nodiscard]] const std::string& getPath() const { return mPath; }
        [[nodiscard]] std::size_t getObjectCount() const { return mObjects.size(); }
        [[nodiscard]] Object* getObject(const std::string& name) const { return mObjects.at(name); }
        [[nodiscard]] const std::map<std::string, Object*>& getObjects() const { return mObjects; }

    private:
        std::string mPath;
        std::map<std::string, Object*> mObjects;
        std::map<std::string, Mesh*> mMeshes;
        std::vector<std::size_t*> mInstanceIDs;
    };

}
