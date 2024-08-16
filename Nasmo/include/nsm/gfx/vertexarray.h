#pragma once

#include <nsm/common.h>
#include <nsm/gfx/opengl.h>

#include <array>

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
            Double = GL_DOUBLE
        };
    
    public:
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

        template <std::size_t N>
        void setLayout(const std::array<Attribute, N>& attributes) {
            this->setLayout(attributes.data(), N);
        }

        [[nodiscard]] u32 getId() const { return mId; }
    
    private:
        void setLayout(const Attribute* attributes, const std::size_t count);

    private:
        u32 mId;
    };

}
