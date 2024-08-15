#include <nsm/gfx/indexbuffer.h>

#include <nsm/gfx/opengl.h>
#include <nsm/debug/log.h>

nsm::IndexBuffer::IndexBuffer()
    : mId(GL_NONE)
    , mCount(0)
{ }

nsm::IndexBuffer::IndexBuffer(const u32* data, const std::size_t size, const BufferUsage usage)
    : mId(GL_NONE)
    , mCount(size / sizeof(u32))
{
    this->init(data, size, usage);
}

nsm::IndexBuffer::~IndexBuffer() {
    nsm::trace("Deleting index buffer with id: ", mId);
    glDeleteBuffers(1, &mId);
}

void nsm::IndexBuffer::init(const u32* data, const std::size_t size, const BufferUsage usage) {
    glCreateBuffers(1, &mId);
    nsm::trace("Creating index buffer with id: ", mId);

    glNamedBufferData(mId, size, data, static_cast<GLenum>(usage));

    mCount = size / sizeof(u32);
}
