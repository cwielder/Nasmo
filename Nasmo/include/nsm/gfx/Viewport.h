#pragma once

#include <glm/vec2.hpp>

namespace nsm {

    class Texture2D;

    class Viewport final {
    public:
        Viewport(const glm::u32vec2& size);
        Viewport(const Texture2D& texture);
        ~Viewport() = default;

        void apply() const;

        void resize(const glm::u32vec2& size) { mSize = size; }
        [[nodiscard]] const glm::u32vec2& getSize() const { return mSize; }

    private:
        glm::u32vec2 mSize;
    };

}
