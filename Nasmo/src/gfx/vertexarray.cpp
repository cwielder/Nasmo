#include <nsm/gfx/vertexarray.h>

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/opengl.h>

#include <nsm/debug/assert.h>

// VertexArray

nsm::VertexArray::VertexArray()
    : mId(GL_NONE)
{
    glCreateVertexArrays(1, &mId);
    nsm::trace("Creating vertex array with id: ", mId);
}

nsm::VertexArray::~VertexArray() {
    NSM_ASSERT(mId != GL_NONE, "VertexArray is not initialized");

    nsm::trace("Deleting vertex array with id: ", mId);
    glDeleteVertexArrays(1, &mId);
}

void nsm::VertexArray::bind() const {
    NSM_ASSERT(mId != GL_NONE, "VertexArray is not initialized");

    glBindVertexArray(mId);
}

void nsm::VertexArray::linkBuffer(const VertexBuffer& buffer, const u32 vboIndex) {
    NSM_ASSERT(vboIndex < 16, "VBO index is out of range");
    NSM_ASSERT(mId != GL_NONE, "VertexArray is not initialized");

    glVertexArrayVertexBuffer(mId, vboIndex, buffer.getId(), 0, buffer.getStride());
}

void nsm::VertexArray::linkIndices(const IndexBuffer& indices) const {
    NSM_ASSERT(mId != GL_NONE, "VertexArray is not initialized");

    glVertexArrayElementBuffer(mId, indices.getId());
}

void nsm::VertexArray::setLayout(const Attribute* attributes, const std::size_t count) {
    NSM_ASSERT(mId != GL_NONE, "VertexArray is not initialized");

    for (std::size_t i = 0; i < count; i++) {
        const Attribute& attribute = attributes[i];

        glEnableVertexArrayAttrib(mId, attribute.location);
        glVertexArrayAttribFormat(mId, attribute.location, attribute.count, static_cast<u32>(attribute.type), attribute.normalized, attribute.offset);
        glVertexArrayAttribBinding(mId, attribute.location, attribute.vboIndex);
    }
}
