#include "ServiceLocator.h"

using namespace dae;

std::unique_ptr<SoundManager> ServiceLocator::m_SoundManagerInstance{ std::make_unique<NullSoundManager>() };

SoundManager& ServiceLocator::GetSoundManagerInstance()
{
	return *m_SoundManagerInstance;
}

SoundManager& ServiceLocator::RegisterSoundManagerInstance(std::unique_ptr<SoundManager>&& soundManager)
{
	m_SoundManagerInstance = std::move(soundManager);
	return GetSoundManagerInstance();
}

//const InputManager& ServiceLocator::GetInputManagerInstance()
//{
//	return *m_InputManagerInstance;
//}
//
//void ServiceLocator::RegisterInputManagerInstance(std::unique_ptr<InputManager>&& inputManager)
//{
//	m_InputManagerInstance = std::move(inputManager);
//}
