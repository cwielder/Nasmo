#include <nsm/gfx/vertexarray.h>

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/opengl.h>

#include <nsm/debug/log.h>

// VertexArray::Attribute

nsm::VertexArray::Attribute::Attribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized)
    : mLocation(location)
    , mCount(count)
    , mType(static_cast<u32>(type))
    , mOffset(offset)
    , mNormalized(normalized)
{ }

void nsm::VertexArray::Attribute::bind(const u32 stride, const u32 vao) const {
    glEnableVertexArrayAttrib(vao, mLocation);
    glVertexArrayAttribFormat(vao, mLocation, mCount, mType, mNormalized, mOffset);
    glVertexArrayAttribBinding(vao, mLocation, 0);
}

// VertexArray

nsm::VertexArray::VertexArray()
    : mId(GL_NONE)
    , mAttributes()
{
    glCreateVertexArrays(1, &mId);
    nsm::trace("Creating vertex array with id: ", mId);
}

nsm::VertexArray::~VertexArray() {
    nsm::trace("Deleting vertex array with id: ", mId);
    glDeleteVertexArrays(1, &mId);
}

void nsm::VertexArray::bind() const {
    glBindVertexArray(mId);

    for (const auto& attribute : mAttributes) {
        attribute.bind(0, mId);
    }
}

void nsm::VertexArray::linkBuffer(const VertexBuffer& buffer) const {
    glVertexArrayVertexBuffer(mId, 0, buffer.getId(), 0, buffer.getStride());
}

void nsm::VertexArray::linkIndices(const IndexBuffer& indices) const {
    glVertexArrayElementBuffer(mId, indices.getId());
}

void nsm::VertexArray::markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized) {
    mAttributes.emplace_back(location, count, type, offset, normalized);
}
