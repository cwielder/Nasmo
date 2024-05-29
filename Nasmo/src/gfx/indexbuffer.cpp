#include <nsm/gfx/indexbuffer.h>

#include <nsm/gfx/opengl.h>

nsm::IndexBuffer::IndexBuffer()
    : mId(GL_NONE)
    , mCount(0)
{ }

nsm::IndexBuffer::IndexBuffer(const u32* data, const u32 size, const BufferUsage usage)
    : mId(GL_NONE)
    , mCount(size / sizeof(u32))
{
    glCreateBuffers(1, &mId);

    glNamedBufferData(mId, size, data, static_cast<GLenum>(usage));
}

nsm::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mId);
}

void nsm::IndexBuffer::init(const u32* data, const u32 size, const BufferUsage usage) {
    glCreateBuffers(1, &mId);

    glNamedBufferData(mId, size, data, static_cast<GLenum>(usage));

    mCount = size / sizeof(u32);
 }

void nsm::IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}
