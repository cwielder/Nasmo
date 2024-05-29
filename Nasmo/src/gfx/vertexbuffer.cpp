#include <nsm/gfx/vertexbuffer.h>

// VertexBuffer::Attribute

nsm::VertexBuffer::Attribute::Attribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized)
    : location_(location)
    , count_(count)
    , type_(static_cast<u32>(type))
    , offset_(offset)
    , normalized_(normalized)
{ }

void nsm::VertexBuffer::Attribute::bind(const u32 stride) const {
    glEnableVertexAttribArray(this->location_);
    glVertexAttribPointer(this->location_, this->count_, this->type_, this->normalized_, stride, reinterpret_cast<void*>(static_cast<u64>(this->offset_)));
}

void nsm::VertexBuffer::Attribute::unbind() const {
    glDisableVertexAttribArray(this->location_);
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
    glGenBuffers(1, &mId);
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));

    mAttributes.reserve(8);
}

nsm::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &mId);
}

void nsm::VertexBuffer::init(const void* data, const u32 size, const u32 stride, const BufferUsage usage) {
    mStride = stride;
    
    glGenBuffers(1, &mId);
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));

    mAttributes.reserve(8);
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

void nsm::VertexBuffer::markAttribute(const u32 count, const DataType type, const bool normalized) {
    u32 offset = 0;

    for (const auto& attribute : mAttributes) {
        offset += (attribute.count_ * attribute.type_);
    }

    mAttributes.emplace_back(VertexBuffer::Attribute(static_cast<u32>(mAttributes.size()), count, type, offset, normalized));
}

void nsm::VertexBuffer::markAttribute(const u32 location, const u32 count, const DataType type, const u32 offset, const bool normalized) {
    mAttributes.emplace_back(VertexBuffer::Attribute(location, count, type, offset, normalized));
}
