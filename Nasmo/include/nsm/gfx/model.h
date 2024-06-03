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

namespace nsm {

    class Model {
    public:
        struct Vertex {
            f32 posX, posY, posZ;
            f32 texU, texV;
            f32 colR, colG, colB, colA;
            f32 nmlX, nmlY, nmlZ;
        };

    public:
        class Mesh {
        public:
            Mesh(std::string_view material, const std::vector<u32>& indices, const VertexBuffer& vertexBuffer);
            ~Mesh();

            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;

            void draw(const RenderInfo& renderInfo, const std::vector<std::size_t*>& instanceIds);

            void growInstanceDataBuffer(const std::size_t newSize);
            void shrinkInstanceDataBuffer(const std::size_t newSize, const std::size_t missingIndex);
            void setInstanceData(const void* data, const std::size_t index);
            void setInstanceDataBufferEntrySize(const std::size_t entrySize) { mInstanceDataBufferEntrySize = entrySize; }

            [[nodiscard]] const Material* getMaterial() const { return mMaterial; }
            [[nodiscard]] Material* getMaterial() { return mMaterial; }

        private:
            Material* mMaterial;
            VertexArray mVertexArray;
            IndexBuffer mIndexBuffer;
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

        template <typename T>
        void setInstanceData(const std::string& meshName, void* data, const std::size_t index) {
            auto it = mMeshes.find(meshName);
            if (it == mMeshes.end()) {
                NSM_ASSERT(false, "Mesh ", meshName, " not found in model ", mPath);
            }

            Mesh& mesh = *it->second;
            
            mesh.setInstanceData(data, index);
            mesh.setInstanceDataBufferEntrySize(sizeof(T)); // TODO: Move this to ModelComponent::setInstanceData so it only happens on init (and then make these non-templates)
        }

        [[nodiscard]] const std::vector<std::size_t*>& getInstanceIDs() { return mInstanceIDs; }
        [[nodiscard]] const std::string& getPath() const { return mPath; }

    private:
        std::string mPath;
        VertexBuffer mVertexBuffer;
        std::map<std::string, Mesh*> mMeshes;
        std::vector<std::size_t*> mInstanceIDs;
    };

}
