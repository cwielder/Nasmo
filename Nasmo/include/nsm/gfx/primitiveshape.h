#pragma once

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/indexbuffer.h>

namespace nsm {

    class PrimitiveShape final {
    public:
        static void init();
        static void destroy();

        [[nodiscard]] static VertexArray& getQuadVAO() { return *sQuadVAO; }
        [[nodiscard]] static IndexBuffer& getQuadIBO() { return *sQuadIBO; }

        [[nodiscard]] static VertexArray& getCubeVAO() { return *sCubeVAO; }
        [[nodiscard]] static IndexBuffer& getCubeIBO() { return *sCubeIBO; }

    private:
        static VertexBuffer* sQuadVBO;
        static VertexArray* sQuadVAO;
        static IndexBuffer* sQuadIBO;

        static VertexBuffer* sCubeVBO;
        static VertexArray* sCubeVAO;
        static IndexBuffer* sCubeIBO;
    };

}
