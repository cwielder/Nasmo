#pragma once

#include <nsm/common.h>
#include <nsm/gfx/shaderstorage.h>

#include <string>

namespace nsm {

    class UIText {
    public:
        explicit UIText(const std::string& text);
        ~UIText() = default;

        void setText(const std::string& text);

        [[nodiscard]] const std::string& getText() const {
            return mText;
        }

    private:
        std::string mText;
        //!ShaderStorage mGlyphCoords;
        //!bool mDirty;
    };

}
