#include <nsm/gfx/vertexbuffer.h>

#include <nsm/debug/log.h>

nsm::VertexBuffer::VertexBuffer()
    : mId(GL_NONE)
{ }

nsm::VertexBuffer::VertexBuffer(const void* data, const std::size_t size, const u32 stride, const BufferUsage usage)
    : mId(GL_NONE)
    , mStride(stride)
{
    this->init(data, size, stride, usage);
}

nsm::VertexBuffer::~VertexBuffer() {
    nsm::trace("Deleting vertex buffer with id: ", mId);
    glDeleteBuffers(1, &mId);
}

void nsm::VertexBuffer::init(const void* data, const std::size_t size, const u32 stride, const BufferUsage usage) {
    glCreateBuffers(1, &mId);
    glNamedBufferData(mId, size, data, static_cast<GLenum>(usage));

    mStride = stride;

    nsm::trace("Creating vertex buffer with id: ", mId);
}

void nsm::VertexBuffer::subData(const void* data, const std::size_t size, const u32 offset) {
    glNamedBufferSubData(mId, offset, size, data);
}

void nsm::VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void* nsm::VertexBuffer::map(const BufferAccess access) const {
    return glMapNamedBuffer(mId, static_cast<GLenum>(access));
}

void nsm::VertexBuffer::unmap() const {
    glUnmapNamedBuffer(mId);
}
