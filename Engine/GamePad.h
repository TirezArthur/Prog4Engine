#pragma once
#include "InputManager.h"
#include <bitset>
#include <array>

#define MAX_GAMEPADS 4

namespace dae
{
	class GamePad
	{
	public:
		explicit GamePad(int8_t deviceId = 0);
		~GamePad();
		GamePad(const GamePad& other) = delete;
		GamePad(GamePad&& other) = delete;
		GamePad& operator=(const GamePad& other) = delete;
		GamePad& operator=(GamePad&& other) = delete;
		
		bool UpdateState(TriggerType& trigger, GamepadButton& button);
		int8_t GetDeviceId() const;
		std::array<bool, 16> GetButtonState() const;
	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}