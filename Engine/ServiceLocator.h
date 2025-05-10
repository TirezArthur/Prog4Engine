#pragma once
#include <memory>
#include "SoundManager.h"

namespace dae
{
	class NullSoundManager final : public SoundManager {
		virtual void Play(SoundRequest) override {}
	};
	class InputManager;

	class ServiceLocator final
	{
	public:
		ServiceLocator() = delete;
		ServiceLocator(const ServiceLocator& other) = delete;
		ServiceLocator(ServiceLocator&& other) = delete;
		ServiceLocator& operator=(const ServiceLocator& other) = delete;
		ServiceLocator& operator=(ServiceLocator&& other) = delete;

		static SoundManager& GetSoundManagerInstance();
		static void RegisterSoundManagerInstance(std::unique_ptr<SoundManager>&& soundManager);

		//static const InputManager& GetInputManagerInstance();
		//static void RegisterInputManagerInstance(std::unique_ptr<InputManager>&& inputManager);
	private:
		static std::unique_ptr<SoundManager> m_SoundManagerInstance;
		//static std::unique_ptr<InputManager> m_InputManagerInstance;
	};
}
