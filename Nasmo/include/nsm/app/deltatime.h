#pragma once

#include <nsm/common.h>

#include <chrono>

namespace nsm {

    class DeltaTime final {
    public:
        f32 update();

    private:
        std::chrono::time_point<std::chrono::steady_clock> mLastTime;
        f32 mTimeStep;
    };

}
