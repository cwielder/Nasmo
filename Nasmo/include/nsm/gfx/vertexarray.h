#pragma once

#include <nsm/common.h>

namespace nsm {

    class VertexBuffer;
    class IndexBuffer;

    class VertexArray final {
        NSM_NO_COPY(VertexArray);

    public:
        VertexArray();
        ~VertexArray();

        void bind() const;

        void linkBuffer(const VertexBuffer& buffer) const;
        void linkIndices(const IndexBuffer& indices) const;
        
    private:
        u32 mId;
    };

}
