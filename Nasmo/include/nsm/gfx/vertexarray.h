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
        struct Attribute {
            u32 location;
            u32 count;
            DataType type;
            u32 offset;
            u32 vboIndex;
            bool normalized;
        };

    public:
        VertexArray();
        ~VertexArray();

        void bind() const;

        void linkBuffer(const VertexBuffer& buffer, const u32 vboIndex);
        void linkIndices(const IndexBuffer& indices) const;
        
        void markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const u32 vboIndex, const bool normalized = false);

    private:
        u32 mId;
        std::vector<Attribute> mAttributes;
    };

}
