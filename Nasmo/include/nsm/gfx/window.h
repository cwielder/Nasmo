#pragma once

namespace nsm {

    class Window {
    public:
        enum class WindowType {
            Windowed,
            StaticWindowed,
            Borderless,
            Fullscreen,

            Count
        };
    };

}
