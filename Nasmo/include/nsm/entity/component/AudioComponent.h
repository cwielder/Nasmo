#pragma once

#include <nsm/entity/Entity.h>

#include <string>
#include <vector>
#include <tuple>

namespace nsm {

    class TransformComponent;
    class SoundHandle;

    class AudioComponent : public EntityComponent {
    public:
        AudioComponent(const std::vector<std::string>& sounds, const TransformComponent* transform);
        ~AudioComponent() override = default;

        void startSound(const std::string& sound, std::unique_ptr<SoundHandle>* handle);

        [[nodiscard]] bool isInited() const { return mInited; }
        [[nodiscard]] const std::vector<std::string>& getSounds() const { return mSounds; }
        [[nodiscard]] const std::vector<std::pair<std::unique_ptr<SoundHandle>*, std::string>>& getRequests() const { return mRequests; }
        [[nodiscard]] const TransformComponent* getTransform() const { return mTransform; }

    private:
        friend class Audio;

        void setInited(bool inited) { mInited = inited; }
        void clearRequests() { mRequests.clear(); }

    private:
        std::vector<std::string> mSounds;
        std::vector<std::pair<std::unique_ptr<SoundHandle>*, std::string>> mRequests;
        const TransformComponent* mTransform;
        bool mInited;
    };

}
