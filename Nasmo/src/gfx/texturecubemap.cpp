#include <nsm/gfx/texturecubemap.h>
#include <nsm/debug/assert.h>

#include <stb_image.h>

nsm::TextureCubemap::TextureCubemap(const std::string& path, const std::string& extension, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter)
    : mId()
{
    NSM_ASSERT(path.back() == '/', "Path must end with a '/'");

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mId);
    
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(enlargeFilter));
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(shrinkFilter));
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::string filename0 = path + "0" + extension;
    i32 width1, height1, channels1;
    stbi_set_flip_vertically_on_load(false);
    u8* data1 = stbi_load(filename0.c_str(), &width1, &height1, &channels1, 0);
    NSM_ASSERT(data1, "Failed to load image: " + filename0);

    GLenum format = GL_RGB8;
    switch (channels1) {
        case 1: format = GL_R8; break;
        case 3: format = GL_RGB8; break;
        case 4: format = GL_RGBA8; break;
    }

    glTextureStorage2D(mId, 1, format, width1, height1);

    GLenum format2 = GL_RGB;
    switch (format) {
        case GL_R8: format2 = GL_RED; break;
        case GL_RGB8: format2 = GL_RGB; break;
        case GL_RGBA8: format2 = GL_RGBA; break;
    }

    glTextureSubImage3D(mId, 0, 0, 0, 0, width1, height1, 1, format2, GL_UNSIGNED_BYTE, data1);

    stbi_image_free(data1);

    for (std::int_fast8_t i = 1; i < 6; i++) {
        std::string filename = path + std::to_string(i) + extension;

        i32 width = 0, height = 0, channels = 0;

        stbi_set_flip_vertically_on_load(false);
        u8* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        NSM_ASSERT(data, "Failed to load image: " + filename);

        glTextureSubImage3D(mId, 0, 0, 0, i, width, height, 1, format2, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
}

nsm::TextureCubemap::~TextureCubemap() {
    glDeleteTextures(1, &mId);
}

void nsm::TextureCubemap::bind(const u32 slot) const {
    glBindTextureUnit(slot, mId);
}
