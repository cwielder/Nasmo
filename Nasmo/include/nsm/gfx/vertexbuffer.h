#pragma once

#include <nsm/common.h>

#include <nsm/gfx/bufferusage.h>
#include <nsm/gfx/opengl.h>

#include <vector>

namespace nsm {

    class VertexBuffer final {
    public:
        enum class DataType {
            Byte = GL_BYTE,
            UnsignedByte = GL_UNSIGNED_BYTE,
            Short = GL_SHORT,
            UnsignedShort = GL_UNSIGNED_SHORT,
            Int = GL_INT,
            UnsignedInt = GL_UNSIGNED_INT,
            UnsignedInt24_8 = GL_UNSIGNED_INT_24_8,
            Float = GL_FLOAT,
            Double = GL_DOUBLE,

            Count
        };

    private:
        class Attribute {
        public:
            Attribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized);
            ~Attribute() = default;

            void bind(const u32 stride) const;
            void unbind() const;

            u32 mLocation;
            u32 mCount;
            u32 mType;
            u32 mOffset;
            bool mNormalized;
        };

    public:
        VertexBuffer();
        VertexBuffer(const void* data, const u32 size, const u32 stride, const BufferUsage usage);
        
        ~VertexBuffer();

        void init(const void* data, const u32 size, const u32 stride, const BufferUsage usage);

        void subData(const void* data, const u32 size, const u32 offset);
        
        void bind() const;
        void unbind() const;

        void markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized = false);
    
    private:
        u32 mId;
        u32 mStride;
        std::vector<Attribute> mAttributes;
    };

}
