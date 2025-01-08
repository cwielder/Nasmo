#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/OpenGL.h>
#include <nsm/gfx/TextureEnum.h>

#include <string>

namespace nsm {

    class TextureCubemap {
        NSM_NO_COPY(TextureCubemap);

    public:
        /**
         * @param path The path to the folder containing the 6 images of the cubemap, named 0-5
         */
        TextureCubemap(const std::string& path, const std::string& extension = ".png", const Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear, const Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear);
        ~TextureCubemap();

        TextureCubemap(TextureCubemap&& other) noexcept
            : mId(other.mId)
        {
            other.mId = GL_NONE;
        }

        TextureCubemap& operator=(TextureCubemap&& other) noexcept {
            if (this != &other) {
                mId = other.mId;

                other.mId = GL_NONE;
            }

            return *this;
        }

        void bind(const u32 slot) const;

    private:
        u32 mId;
    };

}
