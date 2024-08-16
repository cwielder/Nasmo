#include <nsm/gfx/indexbuffer.h>

#include <nsm/gfx/opengl.h>
#include <nsm/debug/assert.h>

#include <limits>

nsm::IndexBuffer::IndexBuffer()
    : mId(GL_NONE)
    , mCount(0)
    , mPrimitiveType(PrimitiveType::Triangles)
{ }

nsm::IndexBuffer::IndexBuffer(const u32* data, const std::size_t size, const BufferUsage usage)
    : mId(GL_NONE)
    , mCount(static_cast<u32>(size / sizeof(u32)))
    , mPrimitiveType(PrimitiveType::Triangles)
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

    NSM_ASSERT(data != nullptr, "Data is nullptr");
    NSM_ASSERT((size / sizeof(u32)) <= std::numeric_limits<u32>::max(), "Size is too large");
    NSM_ASSERT(size > 0, "Size is 0");

    glNamedBufferData(mId, size, data, static_cast<GLenum>(usage));

    mCount = static_cast<u32>(size / sizeof(u32));
}

void nsm::IndexBuffer::draw() const {
    NSM_ASSERT(mId != GL_NONE, "Index buffer is not initialized");

    glDrawElements(static_cast<GLenum>(mPrimitiveType), mCount, GL_UNSIGNED_INT, nullptr);
}

void nsm::IndexBuffer::drawInstanced(const std::size_t instanceCount) const {
    NSM_ASSERT(mId != GL_NONE, "Index buffer is not initialized");
    NSM_ASSERT(instanceCount > 0, "Instance count is 0");
    NSM_ASSERT(instanceCount <= std::numeric_limits<u32>::max(), "Instance count is too large");

    glDrawElementsInstanced(static_cast<GLenum>(mPrimitiveType), mCount, GL_UNSIGNED_INT, nullptr, static_cast<u32>(instanceCount));
}
