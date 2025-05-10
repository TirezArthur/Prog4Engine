#include "SdlSoundManager.h"
#include <SDL_mixer.h>
#include <queue>
#include <condition_variable>
#include <functional>
#include <iostream>


using namespace dae;
using namespace std::placeholders;

class SdlSoundManager::Impl final {
public:
	Impl() : m_ConsumerThread{ std::bind(std::mem_fn(&Impl::ConsumeSound), this, _1) } {};
	~Impl();

	void Play(SoundRequest request);
private:
	void ConsumeSound(std::stop_token stopToken);

	std::queue<SoundRequest> m_SoundQueue;
	std::jthread m_ConsumerThread;
	std::mutex m_QueueLock;
	std::condition_variable m_QueueConditional;
};

SdlSoundManager::Impl::~Impl()
{
	m_ConsumerThread.request_stop();
	m_QueueConditional.notify_all();
}

void SdlSoundManager::Impl::Play(SoundRequest request)
{
	m_SoundQueue.push(request);
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

		std::cout << "Played sound: " << request.soundName << std::endl;
	}
}

dae::SdlSoundManager::SdlSoundManager() : m_Impl{ std::make_unique<SdlSoundManager::Impl>() } 
{
};

dae::SdlSoundManager::~SdlSoundManager() = default;

void SdlSoundManager::Play(SoundRequest request)
{
	m_Impl->Play(request);
}

