#pragma once

#include <nsm/common.h>
#include <nsm/gfx/bufferusage.h>

namespace nsm {

    class IndexBuffer final {
    public:
        IndexBuffer();
        IndexBuffer(const u32* data, const std::size_t size, const BufferUsage usage = BufferUsage::StaticDraw);
        ~IndexBuffer();

        void init(const u32* data, const std::size_t size, const BufferUsage usage = BufferUsage::StaticDraw);
        void bind() const;

        [[nodiscard]] u32 getId() const { return mId; }
        [[nodiscard]] std::size_t getCount() const { return mCount; }

    private:
        u32 mId;
        std::size_t mCount;
    };

}
