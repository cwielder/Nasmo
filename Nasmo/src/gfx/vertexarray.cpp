#include <nsm/gfx/vertexarray.h>

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/opengl.h>

#include <nsm/debug/log.h>

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
        glEnableVertexArrayAttrib(mId, attribute.location);
        glVertexArrayAttribFormat(mId, attribute.location, attribute.count, static_cast<u32>(attribute.type), attribute.normalized, attribute.offset);
        glVertexArrayAttribBinding(mId, attribute.location, 0);
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
