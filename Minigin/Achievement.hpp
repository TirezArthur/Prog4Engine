#pragma once
#include "Observer.hpp"
#include <functional>
#include <vector>
#include <memory>
#include <steam_api.h>
#include <string_view>

namespace dae 
{
	class Achievement final : public Observer
	{
	public:
		static Achievement* Add(const std::string_view id, std::function<bool(const Event&)> eventCheck)
		{
			m_Achievements.emplace_back(new Achievement{ id, eventCheck });
			return m_Achievements.back().get();
		}

		~Achievement() = default;
		Achievement(const Achievement& other) = delete;
		Achievement(Achievement&& other) = delete;
		Achievement& operator=(const Achievement& other) = delete;
		Achievement& operator=(Achievement&& other) = delete;

		void OnNotify(const Event& event) override
		{
			if (!m_Completed && m_EventCheck(event)) {
				m_Completed = true;
				SteamUserStats()->SetAchievement(m_Id.data());
				SteamUserStats()->StoreStats();
				//TODO maybe unbind from subject after completion
			}
		}
	private:
		explicit Achievement(const std::string_view id, std::function<bool(const Event&)> eventCheck) : m_Id{ id }, m_EventCheck { eventCheck } {};

		bool m_Completed{ false };
		std::function<bool(const Event&)> m_EventCheck;
		const std::string_view m_Id;

		static std::vector<std::unique_ptr<Achievement>> m_Achievements;
	};

	std::vector<std::unique_ptr<Achievement>> Achievement::m_Achievements{};
}