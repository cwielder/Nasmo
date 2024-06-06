#include <nsm/app/deltatime.h>

f32 nsm::DeltaTime::update() {
    auto current = std::chrono::steady_clock::now();
    mTimeStep = std::chrono::duration<f32>(current - mLastTime).count();
    mLastTime = current;

    return mTimeStep;
}
