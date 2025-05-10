#pragma once
#include "SoundManager.h"
#include <memory>

namespace dae
{
	class SdlSoundManager final : public SoundManager
	{
	public:
		explicit SdlSoundManager(std::string_view filePath);
		virtual ~SdlSoundManager() override;

		SdlSoundManager(const SdlSoundManager& other) = delete;
		SdlSoundManager(SdlSoundManager&& other) = delete;
		SdlSoundManager& operator=(const SdlSoundManager& other) = delete;
		SdlSoundManager& operator=(SdlSoundManager&& other) = delete;

		virtual void Play(SoundRequest request) override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}

