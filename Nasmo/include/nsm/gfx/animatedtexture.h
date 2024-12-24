#pragma once

#include <nsm/common.h>
#include <nsm/gfx/texture2D.h>

#include <string>

namespace nsm {

    class AnimatedTexture {
    public:
        AnimatedTexture(const std::string& path, const f32 duration);
        ~AnimatedTexture() = default;

        void bind(const u32 slot) const;

        [[nodiscard]] u32 getFrames() const { return mFrames; }
        [[nodiscard]] f32 getDuration() const { return mDuration; }

    private:
        Texture2D mAtlas;
        f32 mDuration;
        u32 mFrames;
    };

}
