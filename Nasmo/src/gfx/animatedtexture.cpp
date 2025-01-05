#include <nsm/gfx/animatedtexture.h>

#include <nsm/debug/assert.h>

nsm::AnimatedTexture::AnimatedTexture(const std::string& path)
    : mAtlas(path, true) // TODO: Support filter configurations with a TextureProps struct
    , mFrames(0)
{
    u32 textureSize = static_cast<u32>(mAtlas.getSize().x);
    mFrames = mAtlas.getSize().y / textureSize;

    NSM_ASSERT(mFrames > 0, "Invalid animated texture");
}

void nsm::AnimatedTexture::bind(const u32 slot) const {
    mAtlas.bind(slot);
}
