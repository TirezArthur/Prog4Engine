#include <SDL_mixer.h>
#include <queue>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <filesystem>
#include "SdlSoundManager.h"


using namespace dae;
using namespace std::placeholders;

class SdlSoundManager::Impl final {
public:
	Impl(std::string_view filePath);
	~Impl();

	void Play(SoundRequest request);
private:
	void ConsumeSound(std::stop_token stopToken);

	std::queue<SoundRequest> m_SoundQueue;
	std::jthread m_ConsumerThread;
	std::mutex m_QueueLock;
	std::condition_variable m_QueueConditional;
	std::map<uint32_t, Mix_Chunk*> m_AudioMap;
	std::filesystem::path m_AudioPath;
};

SdlSoundManager::Impl::Impl(std::string_view filePath) : 
	m_ConsumerThread{ std::bind(std::mem_fn(&Impl::ConsumeSound), this, _1) }, 
	m_AudioPath{ filePath }
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

SdlSoundManager::Impl::~Impl()
{
	for (auto& chunk : m_AudioMap) Mix_FreeChunk(chunk.second);
	Mix_CloseAudio();
	m_ConsumerThread.request_stop();
	m_QueueConditional.notify_all();
}

void SdlSoundManager::Impl::Play(SoundRequest request)
{
	std::unique_lock queueLock{ m_QueueLock };
	m_SoundQueue.push(request);
	queueLock.unlock();
	m_QueueConditional.notify_one();
}

void SdlSoundManager::Impl::ConsumeSound(std::stop_token stopToken)
{
	while (!stopToken.stop_requested()) {
		std::unique_lock queueLock{ m_QueueLock };
		m_QueueConditional.wait(queueLock, [this, &stopToken]() { return !m_SoundQueue.empty() || stopToken.stop_requested(); });

		if (stopToken.stop_requested()) return; //Just in case a stop was requested while waiting

		SoundRequest request = m_SoundQueue.front();
		m_SoundQueue.pop();

		if (m_AudioMap.find(request.soundId) == m_AudioMap.end()) {
			std::filesystem::path filePath = m_AudioPath / request.soundName.data();
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.string().c_str());
			if (chunk == nullptr) {
				std::cout << "Error loading sound: " << request.soundName << " : " << Mix_GetError() << std::endl;
				Mix_ClearError();
			}

			m_AudioMap[request.soundId] = chunk;
		}

		Mix_PlayChannel(-1, m_AudioMap.at(request.soundId), false);
	}
}

dae::SdlSoundManager::SdlSoundManager(std::string_view filePath) : m_Impl{ std::make_unique<SdlSoundManager::Impl>(filePath) }
{
};

dae::SdlSoundManager::~SdlSoundManager() = default;

void SdlSoundManager::Play(SoundRequest request)
{
	m_Impl->Play(request);
}

