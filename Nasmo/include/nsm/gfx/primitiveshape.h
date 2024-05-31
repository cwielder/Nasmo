#pragma once

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/indexbuffer.h>

namespace nsm {

    class PrimitiveShape {
    public:
        static void init();
        static void destroy();

        [[nodiscard]] static VertexArray& getQuadVAO() { return *sQuadVAO; }
        [[nodiscard]] static IndexBuffer& getQuadIBO() { return *sQuadIBO; }

    private:
        static VertexBuffer* sQuadVBO;
        static VertexArray* sQuadVAO;
        static IndexBuffer* sQuadIBO;
    };

}
