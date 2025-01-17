#pragma once

#include <nsm/gfx/VertexBuffer.h>
#include <nsm/gfx/VertexArray.h>
#include <nsm/gfx/IndexBuffer.h>

namespace nsm {

    class PrimitiveShape final {
    public:
        static void init();
        static void destroy();

        [[nodiscard]] static VertexArray& getQuadVAO() { return *sQuadVAO; }
        [[nodiscard]] static IndexBuffer& getQuadIBO() { return *sQuadIBO; }

        [[nodiscard]] static VertexArray& getCubeVAO() { return *sCubeVAO; }
        [[nodiscard]] static IndexBuffer& getCubeIBO() { return *sCubeIBO; }

        [[nodiscard]] static VertexArray& getSphereVAO() { return *sSphereVAO; }
        [[nodiscard]] static IndexBuffer& getSphereIBO() { return *sSphereIBO; }

    private:
        static VertexBuffer* sQuadVBO;
        static VertexArray* sQuadVAO;
        static IndexBuffer* sQuadIBO;

        static VertexBuffer* sCubeVBO;
        static VertexArray* sCubeVAO;
        static IndexBuffer* sCubeIBO;

        static VertexBuffer* sSphereVBO;
        static VertexArray* sSphereVAO;
        static IndexBuffer* sSphereIBO;
    };

}
