#include <nsm/gfx/framebuffer.h>

#include <nsm/debug/assert.h>

#include <glm/gtc/type_ptr.hpp>

nsm::Framebuffer::Framebuffer()
    : mId(GL_NONE)
    , mSize({ 0, 0 })
    , mTextureBuffers()
    , mDepthStencil(nullptr)
    , mFinalized(true)
{ }

nsm::Framebuffer::Framebuffer(const glm::u32vec2& size)
    : mId(GL_NONE)
    , mSize(size)
    , mTextureBuffers()
    , mDepthStencil(nullptr)
    , mFinalized(false)
{
    glCreateFramebuffers(1, &mId);
    NSM_ASSERT(mId != GL_NONE, "Failed to create framebuffer!");

    mTextureBuffers.reserve(8);
}

nsm::Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &mId);

    for (auto& textureBuffer : mTextureBuffers) {
        delete textureBuffer;
    }

    delete mDepthStencil;
}

void nsm::Framebuffer::bind(const BindMode mode) const {
    switch (mode) {
        case BindMode::Draw:  glBindFramebuffer(GL_FRAMEBUFFER, mId); break;
        case BindMode::Read:  glBindFramebuffer(GL_READ_FRAMEBUFFER, mId); break;
        case BindMode::Write: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mId); break;
    }
}

void nsm::Framebuffer::clear(const glm::f32vec4& value, const Type type, const u32 drawBuffer) const {
    if (type & Type::Color) {
        glClearNamedFramebufferfv(mId, GL_COLOR, drawBuffer, glm::value_ptr(value));
    }

    if (type & Type::Depth) {
        glClearNamedFramebufferfv(mId, GL_DEPTH, 0, glm::value_ptr(value));
    }
}

void nsm::Framebuffer::clear(const glm::u32vec4& value, const Type type, const u32 drawBuffer) const {
    if (type & Type::Color) {
        glClearNamedFramebufferuiv(mId, GL_COLOR, drawBuffer, glm::value_ptr(value));
    }
    
    NSM_ASSERT((type & Type::Depth) == 0, "Cannot clear depth buffer with integer value!");
}

void nsm::Framebuffer::resize(const glm::u32vec2& size) {
    NSM_ASSERT(mId != GL_NONE, "Cannot resize backbuffer!");

    mSize = size;

    // as for the texture buffers, we can't resize them so we have to delete them and recreate them

    std::vector<Texture*> newTextureBuffers;
    newTextureBuffers.reserve(mTextureBuffers.size());

    for (auto& textureBuffer : mTextureBuffers) {
        newTextureBuffers.push_back(new Texture(size, textureBuffer->getFormat(), textureBuffer->getFilterMode()));
        glNamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0 + static_cast<u32>(newTextureBuffers.size()) - 1, newTextureBuffers.back()->getID(), 0);
        delete textureBuffer;
    }

    mTextureBuffers = newTextureBuffers;

    if (mDepthStencil != nullptr) {
        Texture* newDepthBuffer = new Texture(size, mDepthStencil->getFormat(), mDepthStencil->getFilterMode());
        glNamedFramebufferTexture(mId, GL_DEPTH_STENCIL_ATTACHMENT, newDepthBuffer->getID(), 0);
        delete mDepthStencil;
        mDepthStencil = newDepthBuffer;
    }
}

const nsm::Framebuffer* nsm::Framebuffer::getBackbuffer() {
    static const Framebuffer backbuffer = Framebuffer();
    return &backbuffer;
}

void nsm::Framebuffer::blit(const Framebuffer& src, const Framebuffer& dst, const glm::u32vec2& srcStart, const glm::u32vec2& srcEnd, const glm::u32vec2& dstStart, const glm::u32vec2& dstEnd, const u32 typeMask, const Texture::FilterMode filterMode) {
    u32 mask = 0;
    if (typeMask & static_cast<u32>(Type::Color)) {
        mask |= GL_COLOR_BUFFER_BIT;
    }
    if (typeMask & static_cast<u32>(Type::Depth)) {
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    glBlitNamedFramebuffer(src.getID(), dst.getID(), srcStart.x, srcStart.y, srcEnd.x, srcEnd.y, dstStart.x, dstStart.y, dstEnd.x, dstEnd.y, mask, static_cast<GLenum>(filterMode));
}

void nsm::Framebuffer::addTextureBuffer(const Texture::Format fmt, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter) {
    NSM_ASSERT(mId != GL_NONE, "Cannot add texture buffer to backbuffer!");
    NSM_ASSERT(mFinalized == false, "Cannot add texture buffer to finalized framebuffer!");

    if (fmt == Texture::Format::Depth24Stencil8 || fmt == Texture::Format::Depth32FStencil8) {
        NSM_ASSERT(mDepthStencil == nullptr, "Framebuffer already has depth/stencil attachment!");
        mDepthStencil = new Texture(mSize, fmt, enlargeFilter);
        glNamedFramebufferTexture(mId, GL_DEPTH_STENCIL_ATTACHMENT, mDepthStencil->getID(), 0);
    } else {
        mTextureBuffers.push_back(new Texture(mSize, fmt, enlargeFilter));
        glNamedFramebufferTexture(mId, GL_COLOR_ATTACHMENT0 + static_cast<u32>(mTextureBuffers.size()) - 1, mTextureBuffers.back()->getID(), 0);
    }
}

void nsm::Framebuffer::finalize() const {
    NSM_ASSERT(mId != GL_NONE, "Cannot finalize backbuffer!");

    u32 attachments[32] = { 0 };

    for (std::uint_fast8_t i = 0; i < mTextureBuffers.size(); i++) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glNamedFramebufferDrawBuffers(mId, static_cast<GLsizei>(mTextureBuffers.size()), attachments);

    u32 status = glCheckNamedFramebufferStatus(mId, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char* statusStr = "Unknown";

        switch (status) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: statusStr = "Not all framebuffer attachments are complete."; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: statusStr = "Framebuffer has no attachments."; break;
        }

        NSM_ASSERT(false, "Framebuffer incomplete: ", statusStr);
    }
}

