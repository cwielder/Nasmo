#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/Texture2D.h>

#include <string>

namespace nsm {

    class AnimatedTexture {
    public:
        AnimatedTexture(const std::string& path);
        ~AnimatedTexture() = default;

        void bind(const u32 slot) const;

        [[nodiscard]] u32 getFrames() const { return mFrames; }

    private:
        Texture2D mAtlas;
        u32 mFrames;
    };

}
