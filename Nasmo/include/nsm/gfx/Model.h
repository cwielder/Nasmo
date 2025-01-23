#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/VertexBuffer.h>
#include <nsm/gfx/IndexBuffer.h>
#include <nsm/gfx/VertexArray.h>
#include <nsm/gfx/Material.h>
#include <nsm/gfx/ShaderStorage.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/debug/Assert.h>
#include <nsm/util/AnyVector.h>

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

                [[nodiscard]] Material* getMaterial() const { return mMaterial; }

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

            [[nodiscard]] const std::vector<Shape*>& getShapes() const { return mShapes; }

        private:
            std::vector<Shape*> mShapes;
        };

        class Object {
        public:
            Object(Object* parent, const fastgltf::Asset& gltf, const std::map<std::string, nsm::Model::Mesh*>& meshes, const glm::mat4& transform, const fastgltf::pmr::MaybeSmallVector<size_t>& children, bool transformOnly = true);
            virtual ~Object();

            virtual void growInstanceDataBuffer(const std::size_t newCount);
            virtual void shrinkInstanceDataBuffer(const std::size_t newCount, const std::size_t missingIndex);
            virtual void setInstanceData(const void* data, const std::size_t index);
            virtual void setInstanceDataBufferEntrySize(const std::size_t) { }
            virtual Mesh* getMesh() { return nullptr; }

            void growTransformBuffer();
            void shrinkTransformBuffer(const std::size_t missingIndex);
            void setTransform(const glm::mat4& transform, const std::size_t index);

            [[nodiscard]] std::vector<Object*> getAllChildren() const;
            [[nodiscard]] const glm::mat4& getTransform() const { return mTransform; }
            [[nodiscard]] const std::map<std::string, Object*>& getChildren() const { return mChildren; }
            [[nodiscard]] bool isTransformOnly() const { return mTransformOnly; }
            [[nodiscard]] Object* getParent() const { return mParent; }
            [[nodiscard]] Object* getChild(const std::string& name) const;
            [[nodiscard]] std::size_t getChildCount() const; // Includes children of children
            [[nodiscard]] std::size_t getChildMeshObjectCount() const; // Includes children of children
            [[nodiscard]] std::vector<glm::mat4>& getTransformBuffer() { return mTransformBuffer; }

        protected:
            friend class Model;

            virtual void drawOpaque(const RenderInfo& renderInfo, const std::size_t instanceCount);
            virtual void drawTranslucent(const RenderInfo& renderInfo, const std::size_t instanceCount);

        protected:
            glm::mat4 mTransform;
            std::vector<glm::mat4> mTransformBuffer;
            std::map<std::string, Object*> mChildren;
            Object* mParent;
            bool mTransformOnly;
        };

        class MeshObject final : public Object {
        public:
            MeshObject(Object* parent, const fastgltf::Asset& gltf, const std::map<std::string, nsm::Model::Mesh*>& meshes, const std::string& meshName, const glm::mat4& transform, const fastgltf::pmr::MaybeSmallVector<size_t>& children);
            ~MeshObject();

            void growInstanceDataBuffer(const std::size_t newCount) override;
            void shrinkInstanceDataBuffer(const std::size_t newCount, const std::size_t missingIndex) override;
            void setInstanceData(const void* data, const std::size_t index) override;
            void setInstanceDataBufferEntrySize(const std::size_t entrySize) override;
            Mesh* getMesh() override { return mMesh; }

        protected:
            void drawOpaque(const RenderInfo& renderInfo, const std::size_t instanceCount) override;
            void drawTranslucent(const RenderInfo& renderInfo, const std::size_t instanceCount) override;

        private:
            glm::mat4 preDraw(const std::size_t instanceCount);

            Mesh* mMesh;
            AnyVector mInstanceDataBuffer;
            bool mInstanceDataDirty;
            ShaderStorage mInstanceDataSSBO;
            ShaderStorage mTransformSSBO;
        };

    public:
        Model(const std::string& path);
        ~Model();

        void drawOpaque(const RenderInfo& renderInfo);
        void drawTranslucent(const RenderInfo& renderInfo);

        void addInstance(std::size_t* outID);
        void removeInstance(std::size_t id);

        void setInstanceData(const std::string& objectName, void* data, const std::size_t index);

        void setTransformAll(const std::size_t instanceID, const glm::mat4& transform);
        void setTransform(const std::size_t instanceID, const std::string& objectName, const glm::mat4& transform);

        void forEachMaterial(const std::function<void(Material*)>& func);

        [[nodiscard]] std::vector<Object*> getAllObjects() const;

        [[nodiscard]] const std::vector<std::size_t*>& getInstanceIDs() { return mInstanceIDs; }
        [[nodiscard]] const std::string& getPath() const { return mPath; }
        [[nodiscard]] std::size_t getObjectCount() const;
        [[nodiscard]] std::size_t getMeshObjectCount() const;
        [[nodiscard]] std::size_t getMeshCount() const { return mMeshes.size(); }
        [[nodiscard]] Object* getObject(const std::string& name) const;
        [[nodiscard]] const std::map<std::string, Object*>& getObjects() const { return mObjects; }

    private:
        std::string mPath;
        std::map<std::string, Object*> mObjects; // Root objects
        std::map<std::string, Mesh*> mMeshes;
        std::vector<std::size_t*> mInstanceIDs;
    };

}
