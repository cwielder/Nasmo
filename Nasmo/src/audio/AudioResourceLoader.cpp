#include <nsm/audio/AudioResourceLoader.h>

#include <nsm/Common.h>
#include <nsm/debug/Assert.h>
#include <nsm/audio/Audio.h>

#include <simdjson.h>

#include <format>

nsm::AudioResourceLoader::~AudioResourceLoader() {
    for (auto& [name, resource] : mResources) {
        delete resource;
    }

    mResources.clear();
}

void nsm::AudioResourceLoader::load(const std::string& name) {
    if (mResources.contains(name)) {
        return;
    }

    Resource* resource = new Resource();
    if (!resource->load(name, std::format("audio/{}/{}.json", name, name))) {
        delete resource;
        return;
    }

    mResources[name] = resource;
}

void nsm::AudioResourceLoader::unload(const std::string& name) {
    auto it = mResources.find(name);
    if (it == mResources.end()) {
        return;
    }

    delete it->second;
    mResources.erase(it);
}

const nsm::AudioResourceLoader::Resource* nsm::AudioResourceLoader::getResource(const std::string& name) const {
    auto it = mResources.find(name);
    if (it == mResources.end()) {
        return nullptr;
    }

    return it->second;
}

// Resource

nsm::AudioResourceLoader::Resource::~Resource() {
    for (SoLoud::AudioSource* audioSource : mAudioSources) {
        delete audioSource;
    }
}

bool nsm::AudioResourceLoader::Resource::load(const std::string& name, const std::string& path) {
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document document = parser.iterate(json);

    bool loop = document["IsLoop"].get_bool().value();
    f64 loopPoint = 0.0f;

    if (loop) {
        loopPoint = document["LoopPoint"].get_double().value();
    }

    try {
        auto files = document["Files"].get_array();

        NSM_ASSERT(files.value().count_elements() <= Audio::cSoundTrackNum, "Too many audio files");

        for (auto file : files) {
            std::string_view fileName = file.value().get_string().value();

            // TODO: Stream
            SoLoud::Wav* source = new SoLoud::Wav();
            mAudioSources.push_back(source);

            if (loop) {
                source->setLooping(true);
                source->setLoopPoint(loopPoint);
            }

            if (source->load(std::format("audio/{}/{}", name, fileName).c_str()) != SoLoud::SO_NO_ERROR) {
                return false;
            }
        }
    } catch (simdjson::simdjson_error& error) {
        NSM_ASSERT(false, "Error parsing audio resource file '", path, "': ", error.what());
    }

    return true;
}
