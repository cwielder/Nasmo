#pragma once

#include <nsm/common.h>
#include <nsm/gfx/opengl.h>

#include <vector>

namespace nsm {

    class IndexBuffer;
    class VertexBuffer;

    class VertexArray final {
        NSM_NO_COPY(VertexArray);

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

            void bind(const u32 stride, const u32 vao) const;

            u32 mLocation;
            u32 mCount;
            u32 mType;
            u32 mOffset;
            bool mNormalized;
        };

    public:
        VertexArray();
        ~VertexArray();

        void bind() const;

        void linkBuffer(const VertexBuffer& buffer) const;
        void linkIndices(const IndexBuffer& indices) const;
        
        void markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized = false);

    private:
        u32 mId;
        std::vector<Attribute> mAttributes;
    };

}
