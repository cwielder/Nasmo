#pragma once

#include <nsm/common.h>
#include <nsm/gfx/bufferusage.h>
#include <nsm/gfx/primitivetype.h>

namespace nsm {

    class IndexBuffer final {
        NSM_NO_COPY_OR_MOVE(IndexBuffer);

    public:
        IndexBuffer();
        IndexBuffer(const u32* data, const std::size_t size, const BufferUsage usage = BufferUsage::StaticDraw);
        ~IndexBuffer();

        void init(const u32* data, const std::size_t size, const BufferUsage usage = BufferUsage::StaticDraw);
        
        void draw() const;
        void drawInstanced(const std::size_t instanceCount) const;

        void setPrimitiveType(const PrimitiveType primitiveType) { mPrimitiveType = primitiveType; }

        [[nodiscard]] PrimitiveType getPrimitiveType() const { return mPrimitiveType; }
        [[nodiscard]] u32 getId() const { return mId; }
        [[nodiscard]] u32 getCount() const { return mCount; }

    private:
        u32 mId;
        u32 mCount;
        PrimitiveType mPrimitiveType;
    };

}
