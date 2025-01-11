#include <nsm/entity/component/AudioComponent.h>

nsm::AudioComponent::AudioComponent(const std::vector<std::string>& sounds, const TransformComponent* transform)
    : mSounds(sounds)
    , mTransform(transform)
    , mInited(false)
{ }

void nsm::AudioComponent::startSound(const std::string& sound, std::unique_ptr<SoundHandle>* handle, f32 speed) {
    mRequests.emplace_back(handle, sound, speed);
}
