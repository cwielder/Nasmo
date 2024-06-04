#pragma once

#include <nsm/common.h>

#include <nsm/gfx/bufferusage.h>
#include <nsm/gfx/opengl.h>

#include <vector>

namespace nsm {

    class VertexBuffer final {
        NSM_NO_COPY(VertexBuffer);

    public:
        VertexBuffer();
        VertexBuffer(const void* data, const std::size_t size, const u32 stride, const BufferUsage usage);
        
        ~VertexBuffer();

        void init(const void* data, const std::size_t size, const u32 stride, const BufferUsage usage);
        void subData(const void* data, const std::size_t size, const u32 offset);
        void bind() const;

        [[nodiscard]] u32 getId() const { return mId; }
        [[nodiscard]] u32 getStride() const { return mStride; }
    
    private:
        u32 mId;
        u32 mStride;
    };

}
