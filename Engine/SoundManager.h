#pragma once
#include <stdint.h>
#include <string_view>
#include "Hash.hpp"

namespace dae
{
	struct SoundRequest {
		consteval explicit SoundRequest(std::string_view soundName, float duration = -1.0f) :
			duration{ duration },
			soundName{ soundName },
			soundId{ dae::sdbm_hash(soundName) }
		{
		}

		const std::string_view soundName{ "" };
		const uint32_t soundId{ UINT32_MAX };
		const float duration{ -1.0f };
	};

	class SoundManager
	{
	public:
		SoundManager() = default;
		virtual ~SoundManager() = default;

		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;

		virtual void Play(SoundRequest request) = 0;
	};
}

