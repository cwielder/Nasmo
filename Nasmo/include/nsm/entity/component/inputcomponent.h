#pragma once

#include <nsm/entity/entity.h>
#include <nsm/event/events.h>

#include <functional>

namespace nsm {

    class InputComponent : public EntityComponent {
    public:
        InputComponent() = default;
        ~InputComponent() override = default;

        void onEvent(const Event* event);

        void setOnKeyPress(const std::function<void(const KeyPressEvent*)>& onKeyPress) { mOnKeyPress = onKeyPress; }
        void setOnKeyRelease(const std::function<void(const KeyReleaseEvent*)>& onKeyRelease) { mOnKeyRelease = onKeyRelease; }
        void setOnKeyRepeat(const std::function<void(const KeyRepeatEvent*)>& onKeyRepeat) { mOnKeyRepeat = onKeyRepeat; }
        void setOnMouseMove(const std::function<void(const MouseMoveEvent*)>& onMouseMove) { mOnMouseMove = onMouseMove; }
        void setOnMousePress(const std::function<void(const MousePressEvent*)>& onMousePress) { mOnMousePress = onMousePress; }
        void setOnMouseRelease(const std::function<void(const MouseReleaseEvent*)>& onMouseRelease) { mOnMouseRelease = onMouseRelease; }
        void setOnMouseScroll(const std::function<void(const MouseScrollEvent*)>& onMouseScroll) { mOnMouseScroll = onMouseScroll; }

    private:
        std::function<void(const KeyPressEvent*)> mOnKeyPress;
        std::function<void(const KeyReleaseEvent*)> mOnKeyRelease;
        std::function<void(const KeyRepeatEvent*)> mOnKeyRepeat;
        std::function<void(const MouseMoveEvent*)> mOnMouseMove;
        std::function<void(const MousePressEvent*)> mOnMousePress;
        std::function<void(const MouseReleaseEvent*)> mOnMouseRelease;
        std::function<void(const MouseScrollEvent*)> mOnMouseScroll;
    };

}
