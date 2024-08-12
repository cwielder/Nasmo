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

#include <fastgltf/core.hpp>

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

        class Object {
        public:
            Object(Object* parent, const fastgltf::Asset& gltf, const std::map<std::string, nsm::Model::Mesh*>& meshes, const glm::mat4& transform, const fastgltf::pmr::MaybeSmallVector<size_t>& children, bool transformOnly = true);
            virtual ~Object();

            virtual void growInstanceDataBuffer(const std::size_t newSize);
            virtual void shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex);
            virtual void setInstanceData(const void* data, const std::size_t index);
            virtual void setInstanceDataBufferEntrySize(const std::size_t) { }

            virtual void drawOpaque(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);
            virtual void drawTranslucent(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);

            [[nodiscard]] const glm::mat4& getTransform() const { return mTransform; }
            [[nodiscard]] const std::map<std::string, Object*>& getChildren() const { return mChildren; }
            [[nodiscard]] bool isTransformOnly() const { return mTransformOnly; }
            [[nodiscard]] Object* getParent() const { return mParent; }
            [[nodiscard]] Object* getChild(const std::string& name) const;
            [[nodiscard]] std::size_t getChildCount() const;

        protected:
            glm::mat4 mTransform;
            std::map<std::string, Object*> mChildren;
            Object* mParent;
            bool mTransformOnly;
        };

        class MeshObject final : public Object {
        public:
            MeshObject(Object* parent, const fastgltf::Asset& gltf, const std::map<std::string, nsm::Model::Mesh*>& meshes, const std::string& meshName, const glm::mat4& transform, const fastgltf::pmr::MaybeSmallVector<size_t>& children);
            ~MeshObject();

            void growInstanceDataBuffer(const std::size_t newSize) override;
            void shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex) override;
            void setInstanceData(const void* data, const std::size_t index) override;
            void setInstanceDataBufferEntrySize(const std::size_t entrySize) override { mInstanceDataBufferEntrySize = entrySize; }

            void drawOpaque(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);
            void drawTranslucent(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);

        private:
            Mesh* mMesh;
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
        [[nodiscard]] std::size_t getObjectCount() const;
        [[nodiscard]] Object* getObject(const std::string& name) const;
        [[nodiscard]] const std::map<std::string, Object*>& getObjects() const { return mObjects; }

    private:
        std::string mPath;
        std::map<std::string, Object*> mObjects; // Root objects
        std::map<std::string, Mesh*> mMeshes;
        std::vector<std::size_t*> mInstanceIDs;
    };

}
