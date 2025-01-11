#pragma once

#include <nsm/Common.h>
#include <nsm/audio/AudioResourceLoader.h>
#include <nsm/audio/SoundHandle.h>
#include <nsm/entity/component/AudioComponent.h>

#include <soloud.h>

#include <memory>
#include <vector>

namespace nsm {

    class Entity;
    class TransformComponent;

    class Audio {
    public:
        struct AudioInfo {
            AudioInfo()
                : sampleRate(48000)
                , bufferSize(480)
                , channelCount(2)
            { }

            u32 sampleRate;
            u32 bufferSize;
            u32 channelCount;
        };

    public:
        Audio(const AudioInfo& info);
        ~Audio();

        void update();
        void transferData(const std::vector<Entity*>& entities);

    public:
        static constexpr u32 cSoundTrackNum = 8;

    private:
        std::unique_ptr<SoundHandle> startSound(const AudioComponent::Request& request, const TransformComponent* transform);

    private:
        SoLoud::Soloud mSoLoud;
        AudioResourceLoader mResourceLoader;
    };

}
