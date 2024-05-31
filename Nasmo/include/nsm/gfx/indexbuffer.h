#pragma once

#include <nsm/common.h>
#include <nsm/gfx/bufferusage.h>

namespace nsm {

    class IndexBuffer final {
    private:
        friend class Model;
        friend class VertexArray;

        IndexBuffer();


    public:
        IndexBuffer(const u32* data, const u32 size, const BufferUsage usage = BufferUsage::StaticDraw);
        ~IndexBuffer();

        void init(const u32* data, const u32 size, const BufferUsage usage = BufferUsage::StaticDraw);
        void bind() const;

        [[nodiscard]] u32 getCount() const { return mCount; }

    private:
        u32 mId;
        u32 mCount;
    };

}
