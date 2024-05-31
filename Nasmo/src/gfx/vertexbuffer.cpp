#include <nsm/gfx/vertexbuffer.h>

#include <nsm/debug/log.h>

// VertexBuffer::Attribute

nsm::VertexBuffer::Attribute::Attribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized)
    : mLocation(location)
    , mCount(count)
    , mType(static_cast<u32>(type))
    , mOffset(offset)
    , mNormalized(normalized)
{ }

void nsm::VertexBuffer::Attribute::bind(const u32 stride) const {
    glEnableVertexAttribArray(mLocation);
    glVertexAttribPointer(mLocation, mCount, mType, mNormalized, stride, reinterpret_cast<void*>(static_cast<u64>(mOffset)));
}

void nsm::VertexBuffer::Attribute::unbind() const {
    glDisableVertexAttribArray(mLocation);
}

// VertexBuffer

nsm::VertexBuffer::VertexBuffer()
    : mId(GL_NONE)
    , mStride(0)
    , mAttributes()
{ }

nsm::VertexBuffer::VertexBuffer(const void* data, const u32 size, const u32 stride, const BufferUsage usage)
    : mId(GL_NONE)
    , mStride(stride)
    , mAttributes()
{
    this->init(data, size, stride, usage);
}

nsm::VertexBuffer::~VertexBuffer() {
    nsm::trace("Deleting vertex buffer with id: ", mId);
    glDeleteBuffers(1, &mId);
}

void nsm::VertexBuffer::init(const void* data, const u32 size, const u32 stride, const BufferUsage usage) {
    mStride = stride;
    
    glGenBuffers(1, &mId);
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));

    mAttributes.reserve(8);

    nsm::trace("Creating vertex buffer with id: ", mId);
}

void nsm::VertexBuffer::subData(const void* data, const u32 size, const u32 offset) {
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void nsm::VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mId);

    for (const auto& attribute : mAttributes) {
        attribute.bind(mStride);
    }
}

void nsm::VertexBuffer::unbind() const {
    for (const auto& attribute : mAttributes) {
        attribute.unbind();
    }

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void nsm::VertexBuffer::markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized) {
    mAttributes.emplace_back(VertexBuffer::Attribute(location, count, type, offset, normalized));
}
