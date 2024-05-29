#pragma once

#include <nsm/common.h>

#include <glm/vec2.hpp>
#include <nsm/gfx/opengl.h>

#include <string>

namespace nsm {

    namespace EventType {
        enum {
            WindowResize,
            WindowMinimize,
            WindowMaximize,

            KeyPress,
            KeyRelease,
            KeyRepeat,

            MousePress,
            MouseRelease,
            MouseMove,
            MouseScroll,

            SceneSwitch,

            Count // Start your own events from here
        };
    } // namespace EventType

    class Event {
    protected:
        Event(const u32 type)
            : mType(type)
        { }

    public:
        virtual ~Event() = default;

        [[nodiscard]] u32 getType() const { return mType; }

    private:
        u32 mType;
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(i32 width, i32 height)
            : Event(EventType::WindowResize)
            , mWidth(width), mHeight(height)
        { }

        ~WindowResizeEvent() override = default;

        [[nodiscard]] glm::u32vec2 getSize() const { return glm::u32vec2(mWidth, mHeight); }

    private:
        i32 mWidth;
        i32 mHeight;
    };

    class WindowMinimizeEvent : public Event {
    public:
        WindowMinimizeEvent()
            : Event(EventType::WindowMinimize)
        { }
    };

    class WindowMaximizeEvent : public Event {
    public:
        WindowMaximizeEvent()
            : Event(EventType::WindowMaximize)
        { }
    };

    class KeyEvent : public Event {
    public:
        KeyEvent(u32 type, i32 key, i32 scancode, i32 action, i32 mods)
            : Event(type)
            , mKey(key), mScanCode(scancode), mAction(action), mMods(mods)
        { }

        ~KeyEvent() override = default;

        [[nodiscard]] i32 getKey() const { return mKey; }
        [[nodiscard]] i32 getScancode() const { return mScanCode; }
        [[nodiscard]] i32 getAction() const { return mAction; }
        [[nodiscard]] i32 getMods() const { return mMods; }

    private:
        i32 mKey;
        i32 mScanCode;
        i32 mAction;
        i32 mMods;
    };

    class KeyPressEvent : public KeyEvent {
    public:
        KeyPressEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(EventType::KeyPress, key, scancode, GLFW_PRESS, mods)
        { }
    };

    class KeyReleaseEvent : public KeyEvent {
    public:
        KeyReleaseEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(EventType::KeyRelease, key, scancode, GLFW_RELEASE, mods)
        { }
    };

    class KeyRepeatEvent : public KeyEvent {
    public:
        KeyRepeatEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(EventType::KeyRepeat, key, scancode, GLFW_REPEAT, mods)
        { }
    };

    class MouseEvent : public Event {
    public:
        MouseEvent(u32 type, i32 button, i32 action, i32 mods)
            : Event(type)
            , mButton(button), mAction(action), mMods(mods)
        { }

        ~MouseEvent() override = default;

        [[nodiscard]] i32 getButton() const { return mButton; }
        [[nodiscard]] i32 getAction() const { return mAction; }
        [[nodiscard]] i32 getMods() const { return mMods; }

    private:
        i32 mButton;
        i32 mAction;
        i32 mMods;
    };

    class MousePressEvent : public MouseEvent {
    public:
        MousePressEvent(i32 button, i32 mods)
            : MouseEvent(EventType::MousePress, button, GLFW_PRESS, mods)
        { }
    };

    class MouseReleaseEvent : public MouseEvent {
    public:
        MouseReleaseEvent(i32 button, i32 mods)
            : MouseEvent(EventType::MouseRelease, button, GLFW_RELEASE, mods)
        { }
    };

    class MouseMoveEvent : public Event {
    public:
        MouseMoveEvent(f64 x, f64 y)
            : Event(EventType::MouseMove)
            , mX(x), mY(y)
        { }

        ~MouseMoveEvent() override = default;

        [[nodiscard]] glm::vec2 getPosition() const { return glm::vec2(mX, mY); }

    private:
        f64 mX;
        f64 mY;
    };

    class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent(f64 x, f64 y)
            : Event(EventType::MouseScroll)
            , mX(x), mY(y)
        { }

        ~MouseScrollEvent() override = default;

        [[nodiscard]] glm::vec2 getOffset() const { return glm::vec2(mX, mY); }

    private:
        f64 mX;
        f64 mY;
    };

    class SceneSwitchEvent : public Event {
    public:
        SceneSwitchEvent(const std::string& path)
            : Event(EventType::SceneSwitch)
            , mPath(path)
        { }

        ~SceneSwitchEvent() override = default;

        [[nodiscard]] const std::string& getPath() const { return mPath; }

    private:
        std::string mPath;
    };

}
